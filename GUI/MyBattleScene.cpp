#include "MyBattleScene.h"
#include "NetworkControllerWrap.h"
#include "MemoryProfiler.h"
#include "AppUtils.h"
#include "Utils.h"
#include "BattleModel.h"
#include "CCAnimatedSprite.h"
#include "CCProgressBar.h"
#include "CCParticleSystemView.h"
#include "spellStatCounter.h"
#include "bufAll.h"
#include "MobFactory.h"
#include "cmdCreateCast.h"
#include "Timer.h"
#include "ClotMana.h"
#include "HeroAI.h"
#include "BulletFactory.h"
#include "MainMenuScene.h"
#include "CCheckpoint.h"

#include "CSoundManager.h"
#include "GlobalMapScene.h"
#include "CSave.h"

using namespace cocos2d;
using namespace std;

MyBattleScene* MyBattleScene::pSelf;

//---------------------------------------------------------------------------------------------------------------------------------
MyBattleScene::MyBattleScene():
pBattleModel(NULL),
activeCard(NULL),
pUnitsLayer(NULL),
pBackground(NULL),
activeCardClickOffset(ccp(0, 0)),
activeCardDefaultPos(ccp(0, 0))
{
    for(int i = 0; i < 9; i++)
        cards[i] = NULL;
    
    //dispersion mobs by Y on lines
    for(int i = 0; i < 3; i++)
        dispersionMobs[i] = 0;
    
    //backlight
    for(int i = 0 ; i < 5 ; i++)
    {
        spawningZones[i] = NULL;
        zoneCP[i] = NULL;
    }
    for(int i = 0 ; i < 4 ; i++)
    {
        zoneFromCPtoCP[i] = NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
MyBattleScene::~MyBattleScene()
{
    CC_SAFE_RELEASE(pBattleModel);
    
    MyBattleScene::pSelf = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
CCScene* MyBattleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    HUDScreen* hudLayer = HUDScreen::getInstance();
    scene->addChild(hudLayer, 100);
    hudLayer->release();
    
    MyBattleScene* sceneLayer = MyBattleScene::create();
    MyBattleScene::pSelf = sceneLayer;
    scene->addChild(sceneLayer, 10);
    
    // return the scene
    return scene;
}

//---------------------------------------------------------------------------------------------------------------------------------
MyBattleScene* MyBattleScene::create()
{
    MyBattleScene *pRet = new MyBattleScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool MyBattleScene::init()
{
    if(!CCLayer::init())
        return false;
    
    isEndScene = false;
    
    //set randomize
    std::srand(time(NULL));
    
    
    //load level config from xml
    char* xml = new char[32];
    //std::sprintf(xml, "levels/lvl_%d.xml", std::rand()%4);
    std::sprintf(xml, "levels/lvl_1.xml");
    const XMLNode& mainNode = Utils::getNodeFormXMLFile(xml, "level");
    
    //add units sublayer
    pUnitsLayer = CCLayerColor::create();
    pUnitsLayer->setAnchorPoint(ccp(0, 0));
    pUnitsLayer->setPosition( Settings::sharedInstance()->getBattleFieldRect().origin );
    pUnitsLayer->setContentSize( Settings::sharedInstance()->getBattleFieldRect().size );
    this->addChild(pUnitsLayer, 100);
    
    
    //create battle model
    if(Settings::sharedInstance()->getLastGameMode() && Settings::sharedInstance()->getLastGameMode() == GM_GLOBAL_MAP)
        pBattleModel = BattleModel::createWithGlobalMap();
    else
        pBattleModel = BattleModel::createWithXML(xml);
    
    CC_ASSERT(pBattleModel);
    
    pBattleModel->setListener(this);
    pBattleModel->pushExecution();
    this->addChild(pBattleModel);
    
    //init heroes
    if(Settings::sharedInstance()->getGameMode() == GM_MULTIPLAYER)
        addHeroesForUsers();
    
    
    //add background sublayer
    std::string bgName = Utils::readString(mainNode, "background");
    
    if(Settings::sharedInstance()->getLastGameMode() && Settings::sharedInstance()->getLastGameMode() == GM_GLOBAL_MAP)
    {
        bgName = "Textures/Scenes/BattleGrounds/";
        bgName.append(MemoryProfiler::getInstance()->battleGround);
    }
    
    pBackground = CCSprite::create(bgName.c_str());
    pBackground->setAnchorPoint(ccp(0, 0));
    pBackground->setPosition(ccp(0, 0));
    this->addChild(pBackground, 0);
    
    
    //shading lines
    CCSprite* shade = CCSprite::create("Textures/black.png");
    shade->setAnchorPoint(ccp(0, 0));
    shade->setPosition(ccp(0, 170));
    shade->setScaleX(HUDScreen::screenWidth);
    shade->setScaleY(Settings::sharedInstance()->getBattleFieldRect().size.height);
    shade->setOpacity(100);
    pBackground->addChild(shade);
    //
    
    
    //clots mana
    XMLNode clotsNode = mainNode.getChildNode("clots");
    chance[0] = atoi(clotsNode.getAttribute("water"));
    chance[1] = atoi(clotsNode.getAttribute("fire"));
    chance[2] = atoi(clotsNode.getAttribute("earth"));
    chance[3]  = atoi(clotsNode.getAttribute("air"));
    
    ClotMana::bindToCharacter(getUserHero());
    CTimer::createOnce(SEL_SCHEDULE( &MyBattleScene::generateClotMana ),
                       std::rand()%(CLOT_GENERATE_MAX_INTERVAL - CLOT_GENERATE_MIN_INTERVAL) + CLOT_GENERATE_MIN_INTERVAL);
    //
    
    
    //release dynamic..
    delete[] xml;
    xml = NULL;
    //
    
    
    //create cards
    float posCorrection = 0;
    int   counter = 0;
    
    //count the number of cards
    for(int i = 0; i < 9; i++)
    {
        if(getUserHero()->chosenCardsID[i] > 0)
            counter++;
    }
    
    //correction for to center screen positioning
    posCorrection = HUDScreen::screenWidth / 2 - (114 / 2) * counter;
    
    //create cards
    counter = 0;
    for(int i = 0; i < 9; i++)
    {
        if(getUserHero()->chosenCardsID[i] > 0)
        {
            cards[counter] = Card::initWithXML("cards.xml", getUserHero()->chosenCardsID[i], getUserHero());
            
            cards[counter]->getBody()->setAnchorPoint(ccp(0, 0));
            cards[counter]->getBody()->setPosition(ccp(114 * counter + posCorrection, -5));
            cards[counter]->getBody()->setZOrder(10);
            HUDScreen::getInstance()->addChild(cards[counter]->getBody());
            
            counter++;
        }
    }
    //
    
    //backlight
    //spawns
    spawningZones[0] = CCSprite::create("Textures/black.png");
    spawningZones[0]->setAnchorPoint(ccp(0,0));
    spawningZones[0]->setPosition(ccp(0, 170));
    spawningZones[0]->setScaleX(185);
    spawningZones[0]->setScaleY(270);
    spawningZones[0]->setVisible(false);
    pBackground->addChild(spawningZones[0]);
    
    spawningZones[1] = CCSprite::create("Textures/black.png");
    spawningZones[1]->setAnchorPoint(ccp(0,0));
    spawningZones[1]->setPosition(ccp(185, 170));
    spawningZones[1]->setScaleX(218);
    spawningZones[1]->setScaleY(270);
    spawningZones[1]->setVisible(false);
    pBackground->addChild(spawningZones[1]);
    
    spawningZones[2] = CCSprite::create("Textures/black.png");
    spawningZones[2]->setAnchorPoint(ccp(0,0));
    spawningZones[2]->setPosition(ccp(403, 170));
    spawningZones[2]->setScaleX(218);
    spawningZones[2]->setScaleY(270);
    spawningZones[2]->setVisible(false);
    pBackground->addChild(spawningZones[2]);
    
    spawningZones[3] = CCSprite::create("Textures/black.png");
    spawningZones[3]->setAnchorPoint(ccp(0,0));
    spawningZones[3]->setPosition(ccp(621, 170));
    spawningZones[3]->setScaleX(218);
    spawningZones[3]->setScaleY(270);
    spawningZones[3]->setVisible(false);
    pBackground->addChild(spawningZones[3]);
    
    spawningZones[4] = CCSprite::create("Textures/black.png");
    spawningZones[4]->setAnchorPoint(ccp(0,0));
    spawningZones[4]->setPosition(ccp(839, 170));
    spawningZones[4]->setScaleX(185);
    spawningZones[4]->setScaleY(270);
    spawningZones[4]->setVisible(false);
    pBackground->addChild(spawningZones[4]);
    
    //fromCPtoCP
    for(int i = 0 ; i < 4 ; i++)
    {
        zoneFromCPtoCP[i] = CCSprite::create("Textures/black.png");
        zoneFromCPtoCP[i]->setAnchorPoint(ccp(0,0));
        zoneFromCPtoCP[i]->setPosition(ccp(160 + i * 191, 170));
        zoneFromCPtoCP[i]->setScaleX(131);
        zoneFromCPtoCP[i]->setScaleY(270);
        zoneFromCPtoCP[i]->setVisible(false);
        pBackground->addChild(zoneFromCPtoCP[i]);
    }
    
    //zoneCP
    for(int i = 0 ; i < 5 ; i++)
    {
        zoneCP[i] = CCSprite::create("Textures/black.png");
        zoneCP[i]->setAnchorPoint(ccp(0,0));
        zoneCP[i]->setPosition(ccp(100 + i * 191, 170));
        zoneCP[i]->setScaleX(60);
        zoneCP[i]->setScaleY(270);
        zoneCP[i]->setVisible(false);
        pBackground->addChild(zoneCP[i]);
    }
    //
    
    //init Checkpoint
    this->initCheckpointsWithXML();
    //
    
    CSoundManager::create()->stopBackgroundMusic();
    this->setTouchEnabled(true);
    
    this->scheduleUpdate();
    
    
    return true;
}


//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onExit()
{
    CCLayer::onExit();
    
    CCDirector::sharedDirector()->purgeCachedData();
    CCLabelBMFont::purgeCachedData();
    CCAnimationCache::purgeSharedAnimationCache();
    CCSpriteFrameCache::purgeSharedSpriteFrameCache();
    CCTextureCache::purgeSharedTextureCache();
    CCTextureCache::sharedTextureCache()->removeAllTextures();
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::initCheckpointsWithXML()
{
    const XMLNode mainNode = Utils::getNodeFormXMLFile("checkpoint.xml", "checkpoints");
    //Add Checkpoint
    for (int i = 0; i < mainNode.nChildNode("checkpoint"); i++)
    {
        const XMLNode& checkpointNode = mainNode.getChildNode("checkpoint", i);
        
        CCheckpoint* tCheckpoint = CCheckpoint::create();
        tCheckpoint->setId(Utils::readInt(checkpointNode, "id"));
        tCheckpoint->setTeam((team_fight)Utils::readInt(checkpointNode, "team"));
        tCheckpoint->setPosition(ccp(Utils::readInt(checkpointNode, "x"), Utils::readInt(checkpointNode, "y")));
        tCheckpoint->setValueAdd(Utils::readFloat(checkpointNode, "valueAdd"));
        tCheckpoint->setValueFull(Utils::readFloat(checkpointNode, "valueFull"));
        tCheckpoint->setDistance(Utils::readFloat(checkpointNode, "distance"));
        this->addChild(tCheckpoint);
        vCheckpoints.push_back(tCheckpoint);
    }
    ////////////////////////////////////////////////
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::update(float _dt)
{
    for (vector<CCheckpoint*>::iterator i = vCheckpoints.begin(); i < vCheckpoints.end(); i++)
    {
        (*i)->update(_dt);
    }
    
    if(!isEndScene)
    {
        if(pBattleModel)
        {
            unitReordering();
            
            //update card cooldown
            Card::shakingArrow(_dt);
            
            //update opponent hero AI
            if(getOpponentHero() && getOpponentHero()->pAI)
                getOpponentHero()->pAI->update(_dt);
            
            
            int teamOne = 0;
            int teamTwo = 0;
            
            for (vector<CCheckpoint*>::iterator i = vCheckpoints.begin(); i < vCheckpoints.end(); i++)
            {
                if((*i)->getOwner() == TF_RED)
                    teamTwo++;
                else if((*i)->getOwner() == TF_BLUE)
                    teamOne++;
                
                if(teamOne > 0 && teamTwo > 0)
                    break;
            }
            
            //check for defeat or victory
            if(teamOne == vCheckpoints.size())
                this->onVictory();
            else if(teamTwo == vCheckpoints.size())
                this->onDefeat();
        }
    }
    
    if(!(CSoundManager::create()->isBackgroundMusicPlaying()))
    {
        if(rand()%10 > 5)
            CSoundManager::create()->playBackgroundMusic("Sounds/iDOLEAST_-_Fire_and_Ice_Forest.wav", true);
        else
            CSoundManager::create()->playBackgroundMusic("Sounds/iDOLEAST_-_Fire_and_Ice_Winter_Highlands.wav", true);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
cocos2d::CCSprite** MyBattleScene::getZoneCP()
{
    return zoneCP;
}

//---------------------------------------------------------------------------------------------------------------------------------
cocos2d::CCSprite** MyBattleScene::getZoneFromCPtoCP()
{
    return zoneFromCPtoCP;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event)
{
    CCTouch* touch = NULL;
    
    CCSetIterator it;
    if(activeCard == NULL)
    {
        for (it = touches->begin(); it != touches->end(); ++it)
        {
            if (!(*it))
                break;
            
            touch = (CCTouch*)(*it);
            
            
            //for deck of card
            for(int i = 0; i < 9; i++)
            {
                if(cards[i] && cards[i]->getBody()->boundingBox().containsPoint(touch->getLocation()) && !cards[i]->getLockState())
                {
                    activeCard = cards[i];
                    activeCardDefaultPos = activeCard->getBody()->getPosition();
                    activeCardClickOffset = ccpSub(activeCard->getBody()->getPosition(), touch->getLocation());
                    activeCard->getBody()->setZOrder(activeCard->getBody()->getZOrder() + 1);
                    break;
                }
            }
            //
            
            
            //for select active magic
            for(int i = 0; i < 4; i++)
            {
                if(HUDScreen::spheres[i]->getBody()->boundingBox().containsPoint(touch->getLocation()))
                    HUDScreen::spheres[i]->setSelected(true);
            }
            //
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event)
{
    CCTouch* touch = NULL;
    
    for(CCSetIterator it = touches->begin(); it != touches->end(); ++it)
    {
        if (!(*it))
            break;
        
        touch = (CCTouch*)(*it);
        
        
        //for deck of card
        if(activeCard)
        {
            if(Settings::sharedInstance()->getGameMode() == GM_MULTIPLAYER)
            {
                //send network query "applyCard" to server
                Wrap->sendApplyCard( activeCard->getCardID(), getTouchesLine(touch->getLocation()));
            }
            else
            {
                //create mob if possible
                MonsterSummon(activeCard->getMobID(), touch->getLocation());
            }
            
            activeCard->getBody()->setPosition(activeCardDefaultPos);
            activeCard->getBody()->setZOrder(activeCard->getBody()->getZOrder() - 1);
            activeCard = NULL;
        }
        //
    }
    
    
    //for create spell attack
    for(int i = 0; i < 4; i++)
    {
        if(HUDScreen::spheres[i]->getSelected())
        {
            if(touch && !HUDScreen::spheres[i]->getBody()->boundingBox().containsPoint(touch->getLocation()))
            {
                if(Settings::sharedInstance()->getGameMode() == GM_MULTIPLAYER)
                {
                    //send network query "applySpell" to server
                    Wrap->sendApplySpell( getUserHero()->chosenSpellsID[i], getTouchesLine(touch->getLocation()) );
                }
                else
                {
                    //create spell if possible
                    spellAttack* sa = getUserHero()->getSpellAttack(this->getActiveSpellId());
                    if(sa->castType == CHECK_POINT)
                    {
                        for(int j = 0; j < 5; j++)
                        {
                            if(zoneCP[j]->boundingBox().containsPoint(touch->getLocation()))
                            {
                                //create spell if possible
                                CCPoint tPoint = ccp(zoneCP[j]->getPosition().x, Settings::sharedInstance()->getBattleFieldRect().size.height);
                                createSpell(tPoint);
                            }
                        }
                    }
                    else if(sa->castType == AREA)
                    {
                        for(int j = 0 ; j < 4 ; j++)
                        {
                            if(zoneFromCPtoCP[j]->boundingBox().containsPoint(touch->getLocation()))
                            {
                                //create spell if possible
                                createSpell(touch->getLocation());
                            }
                        }
                    }
                    else if(sa->castType == TARGET && pBattleModel->getUserHero()->targetSpellTarget)
                        createSpell(touch->getLocation());
                }
                
                //reset select active magic
                HUDScreen::spheres[i]->setSelected(false);
            }
        }
    }
    
    //backlight
    for(int i = 0 ; i < 5 ; i++)
    {
        spawningZones[i]->setVisible(false);
    }
    for(int i = 0 ; i < 5 ; i++)
    {
        zoneCP[i]->setVisible(false);
    }
    for(int i = 0 ; i < 4 ; i++)
    {
        zoneFromCPtoCP[i]->setVisible(false);
    }
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        unit->setAnimationColor((cocos2d::ccColor3B){255, 255, 255});
    }
    //for select active magic
    for(int i = 0; i < 4; i++)
    {
        if(HUDScreen::spheres[i]->getBody()->boundingBox().containsPoint(touch->getLocation()))
            HUDScreen::spheres[i]->setSelected(false);
    }
    //
}

//---------------------------------------------------------------------------------------------------------------------------------
int MyBattleScene::getActiveSpellId()
{
    int spellID = 0;
    if(getUserHero()->getActiveSpellSchool() == "water")
        spellID = getUserHero()->chosenSpellsID[0];
    if(getUserHero()->getActiveSpellSchool() == "fire")
        spellID = getUserHero()->chosenSpellsID[1];
    if(getUserHero()->getActiveSpellSchool() == "earth")
        spellID = getUserHero()->chosenSpellsID[2];
    if(getUserHero()->getActiveSpellSchool() == "air")
        spellID = getUserHero()->chosenSpellsID[3];
    return spellID;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event)
{
    CCTouch* touch = NULL;
    
    CCSetIterator it;
    for (it = touches->begin(); it != touches->end(); ++it)
    {
        if (!(*it))
            break;
        
        touch = (CCTouch*)(*it);
        
        
        for(int i = 0 ; i < 4 ; i++)
        {
            if(HUDScreen::spheres[i]->getSelected())
            {
                HUDScreen::spheres[i]->getSelectedSpell()->setPosition(ccp(touch->getLocation().x - 25, touch->getLocation().y - 25));
            }
        }

        //for deck of card
        if(activeCard)
        {
            activeCard->getBody()->setPosition(ccpAdd(touch->getLocation(), activeCardClickOffset));
            //backlight spawning mobs
            vector<CCheckpoint*> checkpoints = MyBattleScene::pSelf->getCheckpoints();
            vector<CCheckpoint*>::iterator iter = checkpoints.begin();
            for(int i = 0 ; i < 5 ; i++)
            {
                spawningZones[i]->setVisible(false);
                if(spawningZones[i]->boundingBox().containsPoint(touch->getLocation()) && (*iter)->getTeam() == TF_BLUE)
                {
                    spawningZones[i]->setVisible(true);
                }
                iter++;
            }
        }
        //spells backlight
        for(int i = 0 ; i < 4 ; i++)
        {
            if(HUDScreen::spheres[i]->getSelected())
            {
                int spellID = 0;
                spellID = getUserHero()->chosenSpellsID[i];
                if(spellID != 0)
                {
                    spellAttack* sa = getUserHero()->getSpellAttack(spellID);
                    //becklight for spells in the area
                    if(sa->castType == AREA)
                    {
                        for(int j = 0 ; j < 4 ; j++)
                        {
                            zoneFromCPtoCP[j]->setVisible(false);
                            if(zoneFromCPtoCP[j]->boundingBox().containsPoint(touch->getLocation()))
                            {
                                zoneFromCPtoCP[j]->setVisible(true);
                            }
                        }
                    }
                    //becklight for spells from checkpoint to checkpoint
                    if(sa->castType == CHECK_POINT)
                    {
                        for(int j = 0 ; j < 5 ; j++)
                        {
                            zoneCP[j]->setVisible(false);
                            if(zoneCP[j]->boundingBox().containsPoint(touch->getLocation()))
                            {
                                zoneCP[j]->setVisible(true);
                            }
                        }
                    }
                    //becklight for spells in target
                    if(sa->castType == TARGET)
                    {
                        CCObject* obj;
                        AnimatedUnitModel* targ = NULL;
                        
                        pBattleModel->getUserHero()->targetSpellTarget = NULL;
                        float distance = -1;
                        int i = 0;
                        float newDistance = 0;
                        
                        CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
                        {
                            AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
                            
                            if( unit->isAlive() &&
                                unit->isMob() &&
                                unit->getTeam() != getUserHero()->getTeam() )
                            {
                                PhysicUnitModel* body = unit->getPhysicalModel();
                
                                if(body)
                                {
                                    if(i == 0)
                                        distance = body->getRadius();
                                    
                                    cocos2d::CCPoint     actionOrigin = touch->getLocation(); //ccpAdd( _attacker->getPosition(), action->rangePoint );
                                    cocos2d::CCPoint     bodyOrigin = ccp(unit->getPosition().x + 4 * body->getCircleOrigin().x, unit->getPosition().y + 3.2 * body->getCircleOrigin().y );
                                    unit->setAnimationColor((cocos2d::ccColor3B){255, 255, 255});
                                    newDistance = ccpDistance(actionOrigin, bodyOrigin);
                                    
                                    if(newDistance < body->getRadius())
                                    {
                                        if(newDistance < distance)
                                        {
                                            targ = unit;
                                            distance = newDistance;
                                        }
                                    }
                                    i++;
                                }
                            }
                        }
                        
                        if(targ)
                        {
                            pBattleModel->getUserHero()->targetSpellTarget = targ;
                            pBattleModel->getUserHero()->targetSpellTarget->setAnimationColor((cocos2d::ccColor3B){255, 0, 0});
                        }
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::showMessage(AnimatedUnitModel* _unit, const char* _msg, const cocos2d::ccColor3B _color, cocos2d::CCPoint _offset)
{
    CCLabelTTF* label = CCLabelTTF::create(_msg, "Arial", 20);
    
    if(Memory->mirror)
        label->setPosition( ccp(1024 - _offset.x, _offset.y) );
    else
        label->setPosition( ccp(_offset.x, _offset.y) );
    
    label->setColor(_color);
    
    CCFiniteTimeAction* move = CCSpawn::create( CCMoveBy::create(2, ccp(0, 100)),
                                               CCFadeOut::create(2),
                                               NULL );
    label->runAction( CCSequence::create( move,
                                         CCCallFuncND::create(this, callfuncND_selector( MyBattleScene::removeMessageCallback ), label),
                                         NULL) );
    
    HUDScreen::getInstance()->addChild(label);
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::removeUnit(const AnimatedUnitModel*  unit)
{
    CCLog("REMOVE UNIT to layer %d", unit->getID());
    
    pUnitsLayer->removeChildByTag(unit->getID(), true);
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::addUnit(AnimatedUnitModel* unit)
{
    CCLog("ADD UNIT to layer %d", unit->getID());
    
    switch (unit->getType())
    {
        case AUM_TYPE_MOB:
        {
            if(unit->getSkin() != "")
            {
                CCAnimatedSprite* unitSprite = CCAnimatedSprite::create(unit);
                unitSprite->setPosition(unit->getPosition());
                unitSprite->setProgressView(CCProgressBar::create("Textures/GUI/HUD/bar.png"));
                pUnitsLayer->addChild(unitSprite, 0, unit->getID());
            }
            else
                CCLog("warning: mob %d not have animations", unit->getID());
            
            break;
        }
            
        case AUM_TYPE_BULLET:
        {
            if(unit->getSkin() != "")
            {
                CCAnimatedSprite* unitSprite = CCAnimatedSprite::create(unit);
                unitSprite->setPosition(unit->getPosition());
                pUnitsLayer->addChild(unitSprite, 0, unit->getID());
            }
            else
                CCLog("warning: bullet %d not have animations", unit->getID());
            
            break;
        }
            
        case AUM_TYPE_SPELL:
        {
            if (Settings::sharedInstance()->getMagicShowingFormat() == BMS_SHOW_PARTICLE_MAGIC)
            {
                CCParticleSystemView* system = CCParticleSystemView::create(unit);
                if (unit->getMovingSpeed() > 0)
                {
                    float angle = 180 + CC_RADIANS_TO_DEGREES(ccpAngleSigned(ccp(1, 0), unit->getMoveDirection()));
                    system->setAngle(angle);
                }
                system->setPosition(unit->getPosition());
                pUnitsLayer->addChild(system, 100000, unit->getID());
            }
            else
            {
                if(unit->getSkin() != "")
                {
                    CCAnimatedSprite* unitSprite = CCAnimatedSprite::create(unit);
                    unitSprite->setPosition(unit->getPosition());
                    pUnitsLayer->addChild(unitSprite, 100000, unit->getID());
                }
                else
                    CCLog("warning: spell %d not have animations", unit->getID());
            }
        }
            break;
            
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::unitReordering()
{
    CCObject* obj;
    int maxZOrder = Settings::sharedInstance()->getBattleFieldRect().size.height * 100;
    
    CCARRAY_FOREACH(pUnitsLayer->getChildren(), obj)
    {
        AUStateListnerInterface* unit = (AUStateListnerInterface*) obj;
        
        if(unit->getZOrder() != 100000)     //if unit is not spell
            unit->setZOrder(maxZOrder - unit->getPosition().y * 100);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
std::vector<CCheckpoint*>& MyBattleScene::getCheckpoints()
{
    return vCheckpoints;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::createSpell(const cocos2d::CCPoint& _touch)
{
    int line = getTouchesLine(_touch);
    
    if(line > 0)    //if touche to line
    {
        float linesHeight = (Settings::sharedInstance()->getBattleFieldRect().size.height / 3);
        
        //set hero position at needed line
        getUserHero()->setPosition(ccp(0, line * linesHeight - linesHeight / 2 - 20));
        getUserHero()->setLine(line);
        //
        
        spellAttack* sa = getUserHero()->getSpellAttack(this->getActiveSpellId());
        CC_ASSERT(sa);
        
        
        if(getUserHero()->checkManaEnough(getUserHero()->getActiveSpellSchool(), sa->manaCost))    //if enough mana for cast spell
        {
            if(getUserHero()->getCastCmdID() != -1)
                pBattleModel->removeCommand(getUserHero()->getCastCmdID());
            
            cmdCreateCast* add = cmdCreateCast::createTS(pBattleModel, ccp(_touch.x, line * linesHeight - linesHeight / 2 - 20), sa, sa->damageTime);
            pBattleModel->addCommand(add);
            add->release();
            
            getUserHero()->setCastCmdID(add->getID());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
int MyBattleScene::getTouchesLine(const cocos2d::CCPoint _position) const
{
    int line = 0;
    
    if(_position.y > 170 &&
       _position.y < 440)
    {
        line = 1;
        
        if(_position.y > 260)
            line = 2;
        
        if(_position.y > 350)
            line = 3;
    }
    
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
int MyBattleScene::getSituationLine(const cocos2d::CCPoint _position) const
{
    const CCArray* allUnits = pBattleModel->getUnits();
    
/** determine the overall situation
 */
    int friendlies[3] = {0, 0, 0};  //count of friendly units on lines
    int enemies[3] = {0, 0, 0};     //count of enemy units on lines

    CCObject* obj = NULL;
    CCARRAY_FOREACH(allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
        
        CCPoint unitPos = unit->getPosition();
        
        if(unit->isAlive() &&
           unit->getType() == AUM_TYPE_MOB &&
           unitPos.x > _position.x - MOB_BEFORE_VISION_DISTANCE &&
           unitPos.x < _position.x + MOB_AFTER_VISION_DISTANCE )
        {
            int line = 0;
            
            if(unit->getChangingLine() == 0)
                line = unit->getLine();
            else
                line = unit->getChangingLine();

            //consider units
            if(line)
            {
                if(unit->getTeam() == 2)     //enemies
                {
                    if(_position.x < unitPos.x)   //consider only those enemies that are ahead
                        enemies[line - 1]++;
                }
                else                         //friendlies
                    friendlies[line - 1]++;
            }
        }
    }
//

/** logic of change line
 */
    int line = 0;
    
    int strength[3] = {0, 0, 0};
    for(int i = 0; i < 3; i++)
        strength[i] = ((friendlies[i] > 0) ? 1 : 0) - ((enemies[i] > 0) ? 1 : 0);
    
    if(strength[1] == -1)
        line = 2;
    else
    {
        if(strength[0] == -1 && strength[2] == -1)
        {
            if(friendlies[1] == 0)
                line = 2;
            else
            {
                if(friendlies[0] == 0 && friendlies[2] == 0)
                {
                    if(enemies[0] >= enemies[2])
                        line = 1;
                    else
                        line = 3;
                }
                else if(friendlies[0] == 0)
                    line = 1;
                else if(friendlies[2] == 0)
                    line = 3;
            }
        }
        else if(strength[0] == -1)
            line = 1;
        else if(strength[2] == -1)
            line = 3;
        else
        {
            //additional logic
            int cmp = friendlies[1];
            line = 2;
            
            if(friendlies[0] < cmp)
            {
                cmp = friendlies[0];
                line = 1;
            }
            
            if(friendlies[2] < cmp)
            {
                cmp = friendlies[2];
                line = 3;
            }
            //
        }
    }
//
    
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
int MyBattleScene::getMinionPosition(int line)
{
    std::vector<AnimatedUnitModel*>   vMobs;
    
    int offset = 60;
    int x = -1;
    for(int i = 0 ; i < 5 ; i++)
    {
        if(spawningZones[i]->isVisible() && i < 4)
        {
            x = spawningZones[i+1]->getPosition().x - offset;
            
            CCObject* obj;
            CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
            {
                AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
                
                if(line == unit->getLine() &&
                   unit->getType() == AUM_TYPE_MOB &&
                   //unit->getPosition().x > spawningZones[i]->getPosition().x &&
                   unit->getPosition().x < spawningZones[i+1]->getPosition().x)
                {
                    vMobs.push_back(unit);
                }
            } 
        }
    }
    
    AnimatedUnitModel* unit = NULL;
    if(vMobs.size() > 0)
    {
        for (int i = vMobs.size() - 1; i >= 0 ; i--)
        {
            for (int j = 0; j < i ; j++)
            {
                if(vMobs[j]->getPosition().x > vMobs[j + 1]->getPosition().x)
                {
                    unit = vMobs[j];
                    vMobs[j] = vMobs[j + 1];
                    vMobs[j + 1] = unit;
                }
            }
        }
        
        for(int i = vMobs.size() - 1; i >= 0; i--)
        {
            //CCLog("MOB POS X: %d", (int)vMobs[i]->getPosition().x);
            if(i != 0)
            {
                if(vMobs[i]->getPosition().x - MOB_COLLISION_DISTANCE_X > vMobs[i - 1]->getPosition().x + MOB_COLLISION_DISTANCE_X)
                {
                    x = vMobs[i]->getPosition().x - MOB_COLLISION_DISTANCE_X;
                    break;
                }
            }
            else
            {
                x = vMobs[i]->getPosition().x - MOB_COLLISION_DISTANCE_X;
                break;
            }
        } 
    }
    vMobs.clear();
    
    return x;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::MonsterSummon(int mobID, CCPoint position)
{
    CCPoint landing = CCPointZero;
    
    //int line = getTouchesLine(position);  //old gameplay conception
    int line = getSituationLine(position);  //new gameplay conception

    landing.x = getMinionPosition(line);
    if(landing.x <= 0)
    {
        bool checkLine[3] = {false, false, false};
        checkLine[line - 1] = true;
        
        for(int i = 0; i < 3; i++)
        {
            if(!checkLine[i])
            {
                checkLine[i] = true;
                line = i + 1;
                
                landing.x = getMinionPosition(line);
                if(landing.x > 0)
                    break;
            }
        }
    }
    
    if(landing.x <= 0)
        return;

    
    if(line > 0 && landing.x >= 0)
    {
        //use card
        if(activeCard->useCard())
        {
            AnimatedUnitModel* pUnit = MobFactory::createTS(mobID, pBattleModel);             //init unit
            pUnit->setInvoker(getUserHero());
            pUnit->setTeam(1);
            pUnit->setLookMode(AUM_LOOK_LEFT_MODE);
            pUnit->setLine(line);
            pUnit->pBattleModel = pBattleModel;
            landing.y = (Settings::sharedInstance()->getBattleFieldRect().size.height / 3) * (line - 1);
            
            /*
             //set dispersion mobs by Y on lines
             landing.y += 10 * (dispersionMobs[line - 1] + 1);
             
             dispersionMobs[line - 1]++;
             
             if(dispersionMobs[line - 1] == 2)
             dispersionMobs[line - 1] = -1;
             //
             */
            
            pUnit->setPosition(landing);
            pUnit->setMoveTarget(ccp(HUDScreen::screenWidth, landing.y));
            
            
            cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, pUnit, 0);
            pBattleModel->addCommand(add);
            add->release();
            
            pUnit->release();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::generateClotMana()
{
    if(MyBattleScene::pSelf)
    {
        //create clot mana random type under chance[]
        ClotMana* pClot = NULL;
        
        int rnd = std::rand()%100;  //throw Dice
        for(int i = 0; i < 4; i++)
        {
            int range = 0;
            for(int j = 0; j < i + 1; j++)
                range += MyBattleScene::pSelf->chance[j];
            
            if(rnd < range)
            {
                pClot = ClotMana::create( (clot_type)i );
                break;
            }
        }
        //
        
        
        if(pClot)
        {
            //set random position in battle field
            int fieldWidth  = Settings::sharedInstance()->getBattleFieldRect().size.width;
            int fieldHeight = Settings::sharedInstance()->getBattleFieldRect().size.height;
            int clotWidth   = pClot->getBody()->getContentSize().width;
            int clotHeight  = pClot->getBody()->getContentSize().height;
            
            pClot->setAnchorPoint(ccp(0.5, 0.5));
            pClot->setPosition(ccp(std::rand()%(fieldWidth - clotWidth) + clotWidth / 2,
                                   std::rand()%(fieldHeight - clotHeight) + clotHeight / 2 + 185));
            pClot->setZOrder(1000);
            MyBattleScene::pSelf->addChild(pClot);
            //
        }
        
        
        //recursive call this function after random time
        CTimer::createOnce(SEL_SCHEDULE( &MyBattleScene::generateClotMana ),
                           std::rand()%(CLOT_GENERATE_MAX_INTERVAL - CLOT_GENERATE_MIN_INTERVAL) + CLOT_GENERATE_MIN_INTERVAL);
        //
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::replaceSceneCallback()
{
    ClotMana::stack.clear();
    Card::release();
    HUDScreen::releaseInstance();
    
    /*Settings::sharedInstance()->setGameMode(GM_SINGLEPLAYER);
     cocos2d::CCDirector::sharedDirector()->replaceScene(MainMenu::scene());*/
    
    if(Settings::sharedInstance()->getGameMode() == GM_GLOBAL_MAP)
        cocos2d::CCDirector::sharedDirector()->replaceScene(GlobalMap::scene());
    else
        cocos2d::CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
}

//---------------------------------------------------------------------------------------------------------------------------------
// NETWORK for MULTIPLAYER game mode
//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::addHeroesForUsers()
{
    //user hero
    Character* initHero1 = NULL;
    
    if(!Memory->mirror)
        initHero1 = Character::createTSWithXML("characters/char_0.xml", pBattleModel, 201);
    else
        initHero1 = Character::createTSWithXML("characters/char_0.xml", pBattleModel, 202);
    
    CC_ASSERT(initHero1);
    
    ParticleSpellFactory* factory1 = ParticleSpellFactory::createTS(pBattleModel);
    initHero1->setSpellFactory(factory1);
    factory1->release();
    
    pBattleModel->setUserHero(initHero1);
    pBattleModel->addUnit(initHero1, true);
    
    CC_SAFE_RELEASE(initHero1);
    //
    
    //opponent player hero
    Character* initHero2 = NULL;
    
    if(!Memory->mirror)
        initHero2 = Character::createTSWithNetwork(pBattleModel, 202);
    else
        initHero2 = Character::createTSWithNetwork(pBattleModel, 201);
    
    CC_ASSERT(initHero2);
    
    ParticleSpellFactory* factory2 = ParticleSpellFactory::createTS(pBattleModel);
    initHero2->setSpellFactory(factory2);
    factory2->release();
    
    pBattleModel->setOpponentHero(initHero2);
    pBattleModel->addUnit(initHero2, true);
    
    CC_SAFE_RELEASE(initHero2);
    //
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onUnitUpdate(int _unitID, int _posX, int _posY)
{
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _unitID)
        {
            if(Memory->mirror)
                unit->setPosition( ccp(1024 - _posX, _posY) );
            else
                unit->setPosition( ccp(_posX, _posY) );
            
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onChangeState(int _unitID, const char* _state)
{
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _unitID)
        {
            unit->setState(_state);
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onChangeDirection(int _unitID)
{
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _unitID)
        {
            unit->setMoveTarget( ccp(Settings::sharedInstance()->getBattleFieldRect().size.width - unit->getMoveTarget().x,
                                     unit->getMoveTarget().y) );
            (unit->getLookMode() == AUM_LOOK_LEFT_MODE) ? (unit->setLookMode(AUM_LOOK_RIGHT_MODE)) : (unit->setLookMode(AUM_LOOK_LEFT_MODE));
            
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onChangeHp(int _unitID, float _hp)
{
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _unitID)
        {
            unit->setHp(unit->getMaxHp() * _hp);
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onShowMessage(int _posX, int _posY, const char* _msg, const char* _color)
{
    CCLabelTTF* label = CCLabelTTF::create(_msg, "Arial", 20);
    
    if(Memory->mirror)
        label->setPosition( ccp(1024 - _posX, _posY) );
    else
        label->setPosition( ccp(_posX, _posY) );
    
    int r = 0, g = 0, b = 0;
    sscanf(_color, "{%d,%d,%d}", &r, &g, &b);
    label->setColor( (ccColor3B){r, g, b} );
    
    CCFiniteTimeAction* move = CCSpawn::create( CCMoveBy::create(2, ccp(0, 100)),
                                               CCFadeOut::create(2),
                                               NULL );
    label->runAction( CCSequence::create( move,
                                         CCCallFuncND::create(this, callfuncND_selector( MyBattleScene::removeMessageCallback ), label),
                                         NULL) );
    
    HUDScreen::getInstance()->addChild(label);
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::removeMessageCallback(void* _obj)
{
    HUDScreen::getInstance()->removeChild( (CCNode*)_obj );
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onCreateMob(int _unitID, int _invokerID, int _mobID, int _dir)
{
    CCLog("ADD MOB: %d", _unitID);
    
    AnimatedUnitModel* invoker = NULL;
    
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _invokerID)
        {
            invoker = unit;
            break;
        }
    }
    
    assert(invoker);
    
    if(invoker)
    {
        AnimatedUnitModel* unit = MobFactory::createTS(_mobID, pBattleModel, _unitID);
        unit->setInvoker(invoker);
        unit->setSpecialID(_mobID);
        unit->setTeam(invoker->getTeam());
        unit->setPosition(ccp(512, -1000));
        
        if(Memory->mirror)
            unit->setLookMode( (_dir == 1) ? AUM_LOOK_RIGHT_MODE : AUM_LOOK_LEFT_MODE );
        else
            unit->setLookMode( (_dir == 1) ? AUM_LOOK_LEFT_MODE : AUM_LOOK_RIGHT_MODE );
        
        pBattleModel->addUnit(unit, true);
        
        CC_SAFE_RELEASE(unit);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onCreateSpell(int _unitID, int _invokerID, int _spellID, int _dir)
{
    CCLog("ADD SPELL: %d", _unitID);
    
    AnimatedUnitModel* invoker = NULL;
    
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _invokerID)
        {
            invoker = unit;
            break;
        }
    }
    
    assert(invoker);
    
    if(invoker)
    {
        if(invoker->getType() == AUM_TYPE_CHAR)
        {
            Character*      hero = dynamic_cast<Character*>(invoker);
            spellAttack*    sa = hero->getSpellAttack(_spellID);
            
            AnimatedUnitModel* unit = NULL;
            
            if(Memory->mirror)
                unit = hero->createTSSpell(sa, (ccp( ((_dir == 1) ? 0 : 1024), 0)), _unitID);
            else
                unit = hero->createTSSpell(sa, (ccp( ((_dir == 1) ? 1024 : 0), 0)), _unitID);
            
            unit->setInvoker(hero);
            unit->setSpecialID(hero->getSpellAttack(_spellID)->id);
            unit->setTeam(invoker->getTeam());
            unit->setPosition(ccp(512, -1000));
            
            if(Memory->mirror)
                unit->setLookMode( (_dir == 1) ? AUM_LOOK_RIGHT_MODE : AUM_LOOK_LEFT_MODE );
            else
                unit->setLookMode( (_dir == 1) ? AUM_LOOK_LEFT_MODE : AUM_LOOK_RIGHT_MODE );
            
            pBattleModel->addUnit(unit, true);
            
            CC_SAFE_RELEASE(unit);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onCreateBullet(int _unitID, int _invokerID, int _bulletID, int _dir)
{
    CCLog("ADD BULLET: %d", _unitID);
    
    AnimatedUnitModel* invoker = NULL;
    
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _invokerID)
        {
            invoker = unit;
            break;
        }
    }
    
    assert(invoker);
    
    if(invoker)
    {
        AnimatedUnitModel* unit = BulletFactory::createTS(_bulletID, pBattleModel, _unitID);
        unit->setInvoker(invoker);
        unit->setSpecialID(_bulletID);
        unit->setTeam(invoker->getTeam());
        unit->setPosition(ccp(512, -1000));
        
        if(Memory->mirror)
            unit->setLookMode( (_dir == 1) ? AUM_LOOK_RIGHT_MODE : AUM_LOOK_LEFT_MODE );
        else
            unit->setLookMode( (_dir == 1) ? AUM_LOOK_LEFT_MODE : AUM_LOOK_RIGHT_MODE );
        
        pBattleModel->addUnit(unit, true);
        
        CC_SAFE_RELEASE(unit);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onCreateAnimation(int _unitID, int _invokerID, const char* _skin, int _dir)
{
    CCLog("ADD ANIM: %d", _unitID);
    
    AnimatedUnitModel* invoker = NULL;
    
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _invokerID)
        {
            invoker = unit;
            break;
        }
    }
    
    assert(invoker);
    
    if(invoker)
    {
        AnimatedUnitModel* unit = new AnimatedUnitModel(_unitID);
        unit->setSkin(_skin);
        unit->setOriginSkinPoint(CCPointZero);
        unit->setTeam( invoker->getTeam() );
        unit->setPosition( ccp(512, -1000) );
        unit->setType(AUM_TYPE_SPELL);
        unit->setMovingSpeed(0);
        
        if(Memory->mirror)
            unit->setLookMode( (_dir == 1) ? AUM_LOOK_RIGHT_MODE : AUM_LOOK_LEFT_MODE );
        else
            unit->setLookMode( (_dir == 1) ? AUM_LOOK_LEFT_MODE : AUM_LOOK_RIGHT_MODE );
        
        pBattleModel->addUnit(unit, true);
        
        CC_SAFE_RELEASE(unit);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onRemoveUnit(int _unitID)
{
    CCLog("REMOVE UNIT: %d", _unitID);
    
    AnimatedUnitModel* unitForRemove = NULL;
    
    CCObject* obj;
    CCARRAY_FOREACH(pBattleModel->getUnits(), obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if(unit->getID() == _unitID)
        {
            unitForRemove = unit;
            break;
        }
    }
    
    assert(unitForRemove);
    
    if(unitForRemove)
    {
        pBattleModel->removeUnit(unitForRemove, true);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onCardApplySuccess(int _cardID)
{
    for(int i = 0; i < 9; i++)
    {
        if(cards[i] && cards[i]->getCardID() == _cardID)
        {
            cards[i]->useCard();
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onVictory()
{
    if(!isEndScene)
    {
        //stop calculating mechanics
        this->pBattleModel->unscheduleUpdate();
        
        //show end of the battle label
        CCLabelTTF* label = CCLabelTTF::create(Utils::getL10NString("VICTORY").c_str(), "Arial", 48);
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(ccp(512, 384));
        HUDScreen::getInstance()->addChild(label);
        //
        
        MemoryProfiler* mProfiler = MemoryProfiler::getInstance();
        CSave* save = CSave::sharedInstance();
        if(Settings::sharedInstance()->getGameMode() != GM_MULTIPLAYER && MemoryProfiler::getInstance()->curQuestMarker > 0)
        {
            mProfiler->stateHero = SMH_WAIT;
            if(MemoryProfiler::getInstance()->curRoad.size() == 0)
            {
                bool add = true;
                for (int i = 0; i < save->winQuestsMarkers.size(); i++)
                {
                    if(save->winQuestsMarkers[i] == mProfiler->curQuestMarker)
                    {
                        add = false;
                        break;
                    }
                }
                if(add)
                    save->winQuestsMarkers.push_back(mProfiler->curQuestMarker);
                
                for (int i = 0; i < mProfiler->newOpenFog.size(); i++)
                {
                    CSave::sharedInstance()->idNewOpenAreaGround.push_back(mProfiler->newOpenFog[i]);
                }
            }
            mProfiler->newOpenFog.clear();
            Settings::sharedInstance()->setGameMode(GM_GLOBAL_MAP);
        }
        else
        {
            //TODO: multiplayer next scene
        }
        
        CTimer::createOnce(SEL_SCHEDULE( &MyBattleScene::replaceSceneCallback ), 3);
    }
    
    isEndScene = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MyBattleScene::onDefeat()
{
    if(!isEndScene)
    {
        //stop calculating mechanics
        this->pBattleModel->unscheduleUpdate();
        
        //show end of the battle label
        CCLabelTTF* label = CCLabelTTF::create(Utils::getL10NString("DEFEAT").c_str(), "Arial", 48);
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(ccp(512, 384));
        HUDScreen::getInstance()->addChild(label);
        //
        
        MemoryProfiler* mProfiler = MemoryProfiler::getInstance();
        if(Settings::sharedInstance()->getGameMode() != GM_MULTIPLAYER)
        {
            if(mProfiler->whenceStartFight == WSF_MARKER)
                mProfiler->stateHero = SMH_LOOSE;
            else if(mProfiler->whenceStartFight == WSF_ROAD)
                mProfiler->stateHero = SMH_RETREAT;
            Settings::sharedInstance()->setGameMode(GM_GLOBAL_MAP);
        }
        else
        {
            //TODO: multiplayer next scene
        }
        
        CTimer::createOnce(SEL_SCHEDULE( &MyBattleScene::replaceSceneCallback ), 3);
    }
    
    isEndScene = true;
}

//---------------------------------------------------------------------------------------------------------------------------------