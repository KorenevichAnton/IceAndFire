#include "WaterHealSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
WaterHealSpellAI::WaterHealSpellAI():
UnitAI(),
typeUnitAI(WATER_HEAL_SPELL_AI),
position(ccp(0, 0)),
firstCPPosition(ccp(0, 0)),
secondCPPosition(ccp(0, 0)),
initialize(false),
skin1("")
{
}

//---------------------------------------------------------------------------------------------------------------------------------
WaterHealSpellAI::~WaterHealSpellAI()
{
    bufedUnits.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
WaterHealSpellAI* WaterHealSpellAI::createTS(BattleModel* _battleModel)
{
    WaterHealSpellAI* pRet = new WaterHealSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;
    
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterHealSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */
    initCastingArea();
    //indentation from the edge of the screen
    float offset = (Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 4;

    //set spell position
    _unitAI->setPosition(cocos2d::CCPoint( (_unitAI->getTeam() == 1) ? (offset) : (Settings::sharedInstance()->getBattleFieldRect().size.width - offset),
                                          _unitAI->getPosition().y ));


    //set healing attack type
    _unitAI->getAttack()->type = AT_FRIENDLY;
    _unitAI->getAttack()->subtype = "heal";
    //
    
    //modelling
    skin1 = "Textures/Skins/spells/Healing_Cross";
    //

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterHealSpellAI::setPosition(cocos2d::CCPoint _position)
{
    position = _position;
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterHealSpellAI::initCastingArea()
{
    if(position.x > 160)
    {
        firstCPPosition.x = 160;
        secondCPPosition.x = 291;
    }
    if(position.x > 351)
    {
        firstCPPosition.x = 351;
        secondCPPosition.x = 482;
    }
    if(position.x > 542)
    {
        firstCPPosition.x = 542;
        secondCPPosition.x = 673;
    }
    if(position.x > 733)
    {
        firstCPPosition.x = 733;
        secondCPPosition.x = 864;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterHealSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);

    if(_unitAI->isAlive())
    {
        CCObject* obj = NULL;
        CCARRAY_FOREACH(_allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            if( unit->isAlive() &&
                unit->isMob() &&
                unit->getTeam() == _unitAI->getTeam() &&
                //unit->getLine() == _unitAI->getLine() &&
                unit->getPosition().x > firstCPPosition.x && unit->getPosition().x < secondCPPosition.x &&
                !isBufed(unit) )
            {
                //MODELLING
                //create model1
                AnimatedUnitModel* model1 = new AnimatedUnitModel();
                model1->setType(AUM_TYPE_SPELL);
                model1->setPosition(unit->getPosition());
                model1->setSkin(skin1);
                model1->setOriginSkinPoint(ccp(0, -50));
                model1->setState("wait1");
                model1->setAnimationSpeed(40);
                model1->pBattleModel = pBattleModel;
                model1->setLifeTime(1);
                
                cmdAddUnit* add1 = cmdAddUnit::createTS(pBattleModel, model1, 0);
                pBattleModel->addCommand(add1);
                add1->release();
                
                model1->release();
                //
                
                
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();

                bufedUnits.push_back( unit );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool WaterHealSpellAI::isBufed(AnimatedUnitModel* _unit)
{
    for(vector<AnimatedUnitModel*>::const_iterator i = bufedUnits.begin(); i < bufedUnits.end(); i++)
    {
        if((*i) == _unit)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
