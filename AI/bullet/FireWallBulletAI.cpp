#include "FireWallBulletAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
FireWallBulletAI::FireWallBulletAI():
UnitAI(),
initialize(false),
remainedCoolDown(0)
{
    typeUnitAI = FIRE_WALL_BULLET_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
FireWallBulletAI::~FireWallBulletAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
FireWallBulletAI* FireWallBulletAI::createTS(BattleModel* _battleModel)
{
    FireWallBulletAI* pRet = new FireWallBulletAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void FireWallBulletAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits)
{
    /* here a initialize default parameters
    */

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void FireWallBulletAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, allUnits);


    if(_unitAI->isAlive())
    {
        remainedCoolDown -= _dt;

        if(remainedCoolDown <= 0)
        {
            CCObject* obj;
            CCARRAY_FOREACH(allUnits, obj)
            {
                AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
                int priority = 0;
                if( unit->getType() == AUM_TYPE_MOB &&
                    unit->getTeam() != _unitAI->getTeam() &&
                    unit->getLine() == _unitAI->getLine() &&
                    canAttack(_unitAI, unit, priority) )
                {
                    cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                    pBattleModel->addCommand(cmdAtt);
                    cmdAtt->release();
                }
            }

            remainedCoolDown = _unitAI->getAttack()->damageTime;
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
