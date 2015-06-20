#include "PulsarEarthBulletAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
PulsarEarthBulletAI::PulsarEarthBulletAI():
UnitAI(),
initialize(false)
{
    typeUnitAI = PULSAR_EARTH_BULLET_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
PulsarEarthBulletAI::~PulsarEarthBulletAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
PulsarEarthBulletAI* PulsarEarthBulletAI::createTS(BattleModel* _battleModel)
{
    PulsarEarthBulletAI* pRet = new PulsarEarthBulletAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void PulsarEarthBulletAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits)
{
    /* here a initialize default parameters
    */

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void PulsarEarthBulletAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt)
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
