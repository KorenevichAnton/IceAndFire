#include "TrapBulletAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
TrapBulletAI::TrapBulletAI():
UnitAI(),
initialize(false)
{
    typeUnitAI = TRAP_BULLET_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
TrapBulletAI::~TrapBulletAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
TrapBulletAI* TrapBulletAI::createTS(BattleModel* _battleModel)
{
    TrapBulletAI* pRet = new TrapBulletAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void TrapBulletAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits)
{
    /* here a initialize default parameters
    */

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void TrapBulletAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, allUnits);

    if(_unitAI->isAlive())
    {
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
