#include "AirChainSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
AirChainSpellAI::AirChainSpellAI():
UnitAI(),
typeUnitAI(AIR_CHAIN_SPELL_AI),
initialize(false),
remainedCoolDown(0),
remainedCountAttack(0),
skin1(""),
skin2(""),
damageDec(0)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
AirChainSpellAI::~AirChainSpellAI()
{
    attackedUnits.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
AirChainSpellAI* AirChainSpellAI::createTS(BattleModel* _battleModel)
{
    AirChainSpellAI* pRet = new AirChainSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void AirChainSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    remainedCountAttack = 10;
    damageDec = 10;
    
    //modelling
    skin1 = "Textures/Skins/spells/Lightning_Blue";
    skin2 = "Textures/Skins/spells/Lightning_White";
    //

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void AirChainSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);

    if(_unitAI->isAlive() && remainedCountAttack)
    {
        remainedCoolDown -= _dt;

        if(remainedCoolDown <= 0)
        {
            AnimatedUnitModel* target = NULL;       //next target
            AnimatedUnitModel* previous = NULL;     //last attacked target

            if(attackedUnits.capacity() == 0)
                previous = _unitAI;
            else
                previous = (*(attackedUnits.end() - 1));

            //find near target
            CCObject* obj = NULL;
            CCARRAY_FOREACH(_allUnits, obj)
            {
                AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
                if( unit->isAlive() &&
                    unit->isMob() &&
                    unit->getTeam() != _unitAI->getTeam() &&
                    unit->getLine() == _unitAI->getLine() &&
                    !isAttacked(unit) )
                {
                    if(target)
                    {
                        float near = fabs( previous->getPosition().x - target->getPosition().x );
                        float cur = fabs( previous->getPosition().x - unit->getPosition().x );
                        if(cur < near)
                            target = unit;
                    }
                    else
                        target = unit;
                }
            }
            //

            if(target)
            {
                //MODELLING
                    //create model1
                    AnimatedUnitModel* model1 = new AnimatedUnitModel();
                    model1->setType(AUM_TYPE_SPELL);
                    model1->setPosition(target->getPosition());
                    model1->setSkin(skin1);
                    model1->setOriginSkinPoint(ccp(0, -50));
                    model1->setState("wait1");
                    model1->setAnimationSpeed(10);
                    model1->pBattleModel = pBattleModel;
                    model1->setLifeTime(0.8);
                
                    cmdAddUnit* add1 = cmdAddUnit::createTS(pBattleModel, model1, 0);
                    pBattleModel->addCommand(add1);
                    add1->release();
                
                    model1->release();
                
                    skin1 = ""; //for single use first animation

                
                    //create model2
                    AnimatedUnitModel* model2 = new AnimatedUnitModel();
                    model2->setType(AUM_TYPE_SPELL);
                    model2->setPosition(target->getPosition());
                    model2->setSkin(skin2);
                    model2->setOriginSkinPoint(ccp(0, -50));
                    model2->setState("wait1");
                    model2->setAnimationSpeed(10);
                    model2->pBattleModel = pBattleModel;
                    model2->setLifeTime(0.5);
                
                    cmdAddUnit* add2 = cmdAddUnit::createTS(pBattleModel, model2, 0.5);
                    pBattleModel->addCommand(add2);
                    add2->release();
                
                    model2->release();
                //

                
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, target, _unitAI->getAttack(), 0);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();

                //decrease damage after attack
                _unitAI->getAttack()->damage -= _unitAI->getAttack()->damage * damageDec * 0.01;
                _unitAI->getAttack()->damageDelta -= _unitAI->getAttack()->damageDelta * damageDec * 0.01;

                attackedUnits.push_back(target);

                remainedCountAttack--;

                remainedCoolDown = _unitAI->getAttack()->damageTime;
            }
            else
            {
                if(attackedUnits.capacity() > 1)
                {
                    previous = (*(attackedUnits.end() - 1));
                    attackedUnits.clear();  //clear the attacked list for a new wave of attacks
                    attackedUnits.push_back(previous);  //come back the last attacked target
                }
                else
                    remainedCountAttack = 0;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool AirChainSpellAI::isAttacked(AnimatedUnitModel* _unit)
{
    for(vector<AnimatedUnitModel*>::const_iterator i = attackedUnits.begin(); i < attackedUnits.end(); i++)
    {
        if((*i) == _unit)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
