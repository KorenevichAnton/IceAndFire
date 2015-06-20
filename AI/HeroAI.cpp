#include "HeroAI.h"
#include "MobFactory.h"
#include "cmdCreateCast.h"
#include "ElementaryUnitAI.h"
#include "MyBattleScene.h"

USING_NS_CC;

//---------------------------------------------------------------------------------------------------------------------------------
HeroAI::HeroAI():
pCharacter(NULL),
pBattleModel(NULL),
cardsCD(0),
spellsCD(0)
{
    for(int i = 0; i < 9; i++)
        cards[i] = NULL;
    
    for(int i = 0; i < 4; i++)
        spells[i] = NULL;
    
    //dispersion mobs by Y on lines
    for(int i = 0; i < 3; i++)
        dispersionMobs[i] = 0;
    
    
    
    secondsForStart = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------
HeroAI::~HeroAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
HeroAI* HeroAI::createForCharacter(Character* _hero, BattleModel* _battleModel)
{
    HeroAI* pSelf = new HeroAI();
    pSelf->pCharacter = _hero;
    pSelf->pCharacter->pAI = pSelf;
    pSelf->pBattleModel = _battleModel;
    
    
    //create cards
    for(int i = 0; i < 9; i++)
    {
        if(_hero->chosenCardsID[i] > 0)
            pSelf->cards[i] = Card::initWithXML("cards.xml", _hero->chosenCardsID[i], _hero);
    }
    //
    
    //create spells
    for(int i = 0; i < 4; i++)
    {
        spellAttack* spell = pSelf->pCharacter->getSpellAttack(pSelf->pCharacter->chosenSpellsID[i]);
        
        if(spell->schoolName == "water")
            pSelf->spells[0] = spell;
        if(spell->schoolName == "fire")
            pSelf->spells[1] = spell;
        if(spell->schoolName == "earth")
            pSelf->spells[2] = spell;
        if(spell->schoolName == "air")
            pSelf->spells[3] = spell;
    }
    //
    
    return pSelf;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HeroAI::update(float _dt)
{
    //calculate cards common cooldown
    if(cardsCD < _dt)
        cardsCD = 0;
    
    if(cardsCD != 0)
        cardsCD -= _dt;
    //
    
    //calculate spells common cooldown
    if(spellsCD < _dt)
        spellsCD = 0;
    
    if(spellsCD != 0)
        spellsCD -= _dt;
    //
    
    if(secondsForStart <= 2)
        secondsForStart += _dt;
    
    //cards
    if(!cardsCD && secondsForStart >= 2)
    {
        int cardID = 0;
        
        do
        {
            cardID = rand()%9;
        }
        while(!cards[cardID]);
        
        
        if(cards[cardID]->useCard())
        {
            int line = rand()%3 + 1;
            createMob(cards[cardID]->getMobID(), line);
            
            //set common cooldown for use cards
            cardsCD = COMMON_CARD_CD;
        }
    }
    //
    
    
    //spells
    if(!spellsCD)
    {
        int spellNum = 0;
        
        do
        {
           spellNum = rand()%4; 
        }
        while(!spells[spellNum]);
        
                
        if(pCharacter->checkManaEnough(spells[spellNum]->schoolName, spells[spellNum]->manaCost))    //if enough mana for cast spell
        {
            int line = rand()%3 + 1;
            createSpell(spells[spellNum]->id, line);
            
            //set common cooldown for use spells
            spellsCD = COMMON_SPELL_CD;
        }
    }
    //
}

//---------------------------------------------------------------------------------------------------------------------------------
void HeroAI::createMob(int _mobID, int _line)
{
    CCPoint landing;
    landing.x = Settings::sharedInstance()->getBattleFieldRect().size.width - (Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 4;
    landing.y = (Settings::sharedInstance()->getBattleFieldRect().size.height / 3) * (_line - 1);
    
    //set dispersion mobs by Y on lines
    landing.y += 10 * (dispersionMobs[_line - 1] + 1);
    
    dispersionMobs[_line - 1]++;
    
    if(dispersionMobs[_line - 1] == 2)
        dispersionMobs[_line - 1] = -1;
    //
    
    
    //create unit
    AnimatedUnitModel* pUnit = MobFactory::createTS(_mobID, pBattleModel);
    pUnit->setInvoker(pCharacter);
    pUnit->setSpecialID(_mobID);
    
    pUnit->setTeam(2);
    pUnit->setLookMode(AUM_LOOK_RIGHT_MODE);
    pUnit->setLine(_line);
    
    //set position on line
    pUnit->setPosition(landing);
    
    //set move direction
    pUnit->setMoveTarget(ccp((Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 4, landing.y));   //move to user landing zone
    
    //add unit AI
    pUnit->pBattleModel = pBattleModel;
    
    //add to battle model
    cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, pUnit, 0);
    pBattleModel->addCommand(add);
    add->release();

    pUnit->release();
}

//---------------------------------------------------------------------------------------------------------------------------------
void HeroAI::createSpell(int _spellID, int _line)
{
    float linesHeight = Settings::sharedInstance()->getBattleFieldRect().size.height / 3;

    
    //set hero position at needed line
    pCharacter->setPosition(ccp(Settings::sharedInstance()->getBattleFieldRect().size.width,
                                _line * linesHeight - linesHeight / 2 - 20));
    pCharacter->setLine(_line);
    //
    
    if(pCharacter->getCastCmdID() != -1)
        pBattleModel->removeCommand(pCharacter->getCastCmdID());
    
    //FORREMOVE CMD CREATECAST
    //create spell
    spellAttack* spell = pCharacter->getSpellAttack(_spellID);
    pCharacter->setActiveSpellSchool(spell->schoolName);
/////////////////////////////////////////////////////////
    //create spell if possible
    
    MyBattleScene* bScene = MyBattleScene::pSelf;
    bool target = false;
    CCPoint tPoint;
    if(spell->castType == CHECK_POINT)
    {
        cocos2d::CCSprite** zones = bScene->getZoneCP();
        std::vector<CCheckpoint*> checkpoints = bScene->getCheckpoints();
        CCheckpoint* lastCheckpoint = NULL;
        for(int j = 0; j < 5; j++)
        {
            if(checkpoints[j]->getTeam() == TF_RED)
            {
                if(!lastCheckpoint || checkpoints[j]->getPosition().x < lastCheckpoint->getPositionX())
                {
                    lastCheckpoint = checkpoints[j];
                    tPoint = lastCheckpoint->getPosition();
                    target = true;
                }
            }
        }
        if(lastCheckpoint)
        {
            for (int j = 0; j < 5; j++)
            {
                if(zones[j]->boundingBox().containsPoint(ccp(lastCheckpoint->getPositionX(), lastCheckpoint->getPositionY() - 50)))
                {
                    //create spell if possible
                    tPoint = ccp(lastCheckpoint->getPositionX(), lastCheckpoint->getPositionY() - 50);
                }
            }
        }
    }
    else if(spell->castType == AREA)
    {
        cocos2d::CCSprite** zones = bScene->getZoneFromCPtoCP();
        std::vector<CCheckpoint*> checkpoints = bScene->getCheckpoints();
        CCheckpoint* lastCheckpoint = NULL;
        for(int j = 0; j < 4; j++)
        {
            if(checkpoints[j]->getOwner() == TF_RED)
            {
                if(!lastCheckpoint || checkpoints[j]->getPosition().x < lastCheckpoint->getPositionX())
                {
                    lastCheckpoint = checkpoints[j];
                    tPoint = lastCheckpoint->getPosition();
                    target = true;
                }
            }
        }
    }
    else if(spell->castType == TARGET)
    {
        CCObject* obj = NULL;
        const cocos2d::CCArray* carr = pBattleModel->getUnits();
        CCARRAY_FOREACH(carr, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
            if(unit->isAlive() && unit->getLine() == _line && unit->getTeam() == 1)
            {
                pBattleModel->getOpponentHero()->targetSpellTarget = unit;
                target = true;
                break;
            }
        }
    }
    
/////////////////////////////////////////////////////////
    
    if(target)
    {
        cmdCreateCast* add = cmdCreateCast::createTS(pBattleModel, ccp(tPoint.x, _line * linesHeight - linesHeight / 2 - 20), spell, spell->damageTime);
        pBattleModel->addCommand(add);
        add->release();
    
        pCharacter->setCastCmdID(add->getID());
    }
}

//---------------------------------------------------------------------------------------------------------------------------------