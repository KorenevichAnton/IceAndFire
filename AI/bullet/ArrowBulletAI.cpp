#include "ArrowBulletAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
ArrowBulletAI::ArrowBulletAI():
UnitAI()
{
    typeUnitAI = ARROW_BULLET_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
ArrowBulletAI::~ArrowBulletAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
ArrowBulletAI* ArrowBulletAI::createTS(BattleModel* _battleModel)
{
    ArrowBulletAI* pRet = new ArrowBulletAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void ArrowBulletAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt)
{
    if(_unitAI->isAlive())
    {
        //move
        if(_unitAI->getLookMode() == AUM_LOOK_LEFT_MODE)
            _unitAI->setPosition( ccp(_unitAI->getPosition().x + _unitAI->getMovingSpeed() * _dt, _unitAI->getPosition().y) );
        else
            _unitAI->setPosition( ccp(_unitAI->getPosition().x - _unitAI->getMovingSpeed() * _dt, _unitAI->getPosition().y) );


        CCObject* obj;
        CCARRAY_FOREACH(allUnits, obj)
        {
            int priority = 0;
            AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
            if( unit->isAlive() &&
                unit->getTeam() != _unitAI->getTeam() &&
                unit->getLine() == _unitAI->getLine() &&
                unit->getType() == AUM_TYPE_MOB &&
                canAttack(_unitAI, unit, priority) )
            {
                
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();

                cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, 0);
                pBattleModel->addCommand(rem);
                rem->release();
                
                //_unitAI->setHp(0);  //to avoid repeating commands
                _unitAI->setAlive(false);  //to avoid repeating commands
                
                break;
            }
        }

        //border controll
        if (!Settings::sharedInstance()->getDeathRect().containsPoint(_unitAI->getPosition()))
        {
            cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, 0);
            pBattleModel->addCommand(rem);
            rem->release();

            _unitAI->setHp(0);  //to avoid repeating commands
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
