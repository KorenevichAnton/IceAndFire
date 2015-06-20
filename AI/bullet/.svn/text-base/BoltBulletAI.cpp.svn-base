#include "BoltBulletAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
BoltBulletAI::BoltBulletAI():
UnitAI(),
initialize(false),
redoubleRange(0)
{
    typeUnitAI = BOLT_BULLET_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
BoltBulletAI::~BoltBulletAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
BoltBulletAI* BoltBulletAI::createTS(BattleModel* _battleModel)
{
    BoltBulletAI* pRet = new BoltBulletAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BoltBulletAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits)
{
    /* here a initialize default parameters
    */

    redoubleRange = 100;   //radius of a second attack

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BoltBulletAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, allUnits);

    if(_unitAI->isAlive())
    {
        //move
        if(_unitAI->getLookMode() == AUM_LOOK_LEFT_MODE)
            _unitAI->setPosition( ccp(_unitAI->getPosition().x + _unitAI->getMovingSpeed() * _dt, _unitAI->getPosition().y) );
        else
            _unitAI->setPosition( ccp(_unitAI->getPosition().x - _unitAI->getMovingSpeed() * _dt, _unitAI->getPosition().y) );

        
        int priority = 0;
        CCObject* obj;
        CCARRAY_FOREACH(allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
            if( unit->getTeam() != _unitAI->getTeam() &&
                unit->getLine() == _unitAI->getLine() &&
                unit->getType() == AUM_TYPE_MOB &&
                canAttack(_unitAI, unit, priority) )
            {
                //first attack
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();

                //second attack
                CCObject* obj2;
                CCARRAY_FOREACH(allUnits, obj2)
                {
                    AnimatedUnitModel* unit2 = (AnimatedUnitModel*)obj2;
                //for(vector<AnimatedUnitModel*>::const_iterator j = _allUnits.begin();  j < _allUnits.end(); j++)
                //{
                    if( ( unit2->isAlive() ||
                          isCanFinishUndead(_unitAI, unit2, priority) ) &&
                        unit2->getType() == AUM_TYPE_MOB &&
                        unit2 != unit &&
                        unit2->getTeam() != _unitAI->getTeam() &&
                        unit2->getLine() == unit->getLine() &&
                        //check for membership in second attack radius
                        unit2->getPosition().x > unit->getPosition().x - redoubleRange &&
                        unit2->getPosition().x < unit->getPosition().x + redoubleRange )
                    {
                        cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit2, _unitAI->getAttack(), 0);
                        pBattleModel->addCommand(cmdAtt);
                        cmdAtt->release();

                        break;
                    }
                }
                //

                cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, FPS);
                pBattleModel->addCommand(rem);
                rem->release();

                _unitAI->setAlive(false);   //to avoid repeating commands

                break;
            }
        }

        //border controll
        if (!Settings::sharedInstance()->getDeathRect().containsPoint(_unitAI->getPosition()))
        {
            cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, 0);
            pBattleModel->addCommand(rem);
            rem->release();

            _unitAI->setAlive(false);   //to avoid repeating commands
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
