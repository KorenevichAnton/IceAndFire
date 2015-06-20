#include "WaterRainSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
WaterRainSpellAI::WaterRainSpellAI():
UnitAI(),
typeUnitAI(WATER_RAIN_SPELL_AI),
initialize(false),
remainedCoolDown(0),
position(ccp(0, 0)),
firstCPPosition(ccp(0, 0)),
secondCPPosition(ccp(0, 0)),
skin1("")
{
}

//---------------------------------------------------------------------------------------------------------------------------------
WaterRainSpellAI::~WaterRainSpellAI()
{
    attackedUnits.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
WaterRainSpellAI* WaterRainSpellAI::createTS(BattleModel* _battleModel)
{
    WaterRainSpellAI* pRet = new WaterRainSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterRainSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */
    initCastingArea();
    
    //modelling
    skin1 = "Textures/Skins/spells/Ice_Rain";
    //

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterRainSpellAI::setPosition(cocos2d::CCPoint _position)
{
    position = _position;
}

//---------------------------------------------------------------------------------------------------------------------------------
void WaterRainSpellAI::initCastingArea()
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
void WaterRainSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);
    
       
    if(_unitAI->isAlive())
    {
        remainedCoolDown -= _dt;

        if(remainedCoolDown <= 0)
        {
            bool isTarget = false;  //check for existence of not attack mobs

            CCObject* obj = NULL;
            CCARRAY_FOREACH(_allUnits, obj)
            {
                AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;

                if( unit->isAlive() &&
                    unit->isMob() &&
                    unit->getTeam() != _unitAI->getTeam() &&
                    //unit->getLine() == _unitAI->getLine() &&
                    unit->getPosition().x > firstCPPosition.x && unit->getPosition().x < secondCPPosition.x &&
                    !isAttacked(unit) )
                {
                    //MODELLING
                        //create model1
                        AnimatedUnitModel* model1 = new AnimatedUnitModel();
                        model1->setType(AUM_TYPE_SPELL);
                        model1->setPosition(unit->getPosition());
                        model1->setSkin(skin1);
                        model1->setOriginSkinPoint(ccp(0, -50));
                        model1->setState("wait1");
                        model1->setAnimationSpeed(10);
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

                    attackedUnits.push_back( (unit) );
                    isTarget = true;

                    break;
                }
            }

            if(!isTarget)
                attackedUnits.clear();  //if not have targets - clear the attacked list for a new wave of attacks
            else
                remainedCoolDown = _unitAI->getAttack()->damageTime;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool WaterRainSpellAI::isAttacked(AnimatedUnitModel* _unit)
{
    for(vector<AnimatedUnitModel*>::const_iterator i = attackedUnits.begin(); i < attackedUnits.end(); i++)
    {
        if((*i) == _unit)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
