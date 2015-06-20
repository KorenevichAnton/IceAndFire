#include "GarpunBulletAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
GarpunBulletAI::GarpunBulletAI():
UnitAI(),
initialize(false),
splashRange(0)
{
    typeUnitAI = GARPUN_BULLET_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
GarpunBulletAI::~GarpunBulletAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
GarpunBulletAI* GarpunBulletAI::createTS(BattleModel* _battleModel)
{
    GarpunBulletAI* pRet = new GarpunBulletAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void GarpunBulletAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits)
{
    /* here a initialize default parameters
    */

    splashRange = 100;   //splash radius

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void GarpunBulletAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt)
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
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();

                //splash attack
                CCObject* objSp;
                CCARRAY_FOREACH(allUnits, objSp)
                {
                    AnimatedUnitModel* unitSp = (AnimatedUnitModel*)objSp;
                    if( ( unitSp->isAlive() ||
                          isCanFinishUndead(_unitAI, unitSp, 0) ) &&
                        unitSp->getType() == AUM_TYPE_MOB &&
                        unitSp != unit &&
                        unitSp->getTeam() != _unitAI->getTeam() &&
                        unitSp->getLine() == unit->getLine() &&
                        //check for membership in splash radius
                        unitSp->getPosition().x > unit->getPosition().x - splashRange &&
                        unitSp->getPosition().x < unit->getPosition().x + splashRange )
                    {
                        cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unitSp, _unitAI->getAttack(), 0);
                        pBattleModel->addCommand(cmdAtt);
                        cmdAtt->release();
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
