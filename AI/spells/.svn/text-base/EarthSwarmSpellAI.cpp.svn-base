#include "EarthSwarmSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
EarthSwarmSpellAI::EarthSwarmSpellAI():
UnitAI(),
typeUnitAI(EARTH_SWARM_SPELL_AI),
initialize(false)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthSwarmSpellAI::~EarthSwarmSpellAI()
{
    for(vector<attackedUnit*>::iterator i = attackedUnits.begin(); i < attackedUnits.end(); i++)
    {
        delete (*i);
        (*i) = NULL;
    }
    attackedUnits.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthSwarmSpellAI* EarthSwarmSpellAI::createTS(BattleModel* _battleModel)
{
    EarthSwarmSpellAI* pRet = new EarthSwarmSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthSwarmSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthSwarmSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);

    if(_unitAI->isAlive())
    {
        //move spell
        _unitAI->setPosition(ccpAdd(_unitAI->getPosition(), ccpMult(_unitAI->getMoveDirection(),_unitAI->getMovingSpeed()*_dt)));

        //calculate cooldowns
        for(vector<attackedUnit*>::iterator i = attackedUnits.begin(); i < attackedUnits.end(); i++)
            (*i)->remainedCoolDown -= _dt;

        //cause damage
        CCObject* obj = NULL;
        CCARRAY_FOREACH(_allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            int priority = 0;
            if( (unit->isAlive()) &&
                (unit->getTeam() != _unitAI->getTeam()) &&
                (unit->isMob()) &&
                (UnitAI::canAttack(_unitAI, unit, priority)) )
            {
                attackedUnit* attacked = checkAttacked( unit );

                if(attacked->remainedCoolDown <= 0)
                {
                    cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                    pBattleModel->addCommand(cmdAtt);
                    cmdAtt->release();

                    attacked->remainedCoolDown = _unitAI->getAttack()->damageTime;
                }
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
attackedUnit* EarthSwarmSpellAI::checkAttacked(AnimatedUnitModel* _unit)
{
    for(vector<attackedUnit*>::const_iterator i = attackedUnits.begin(); i < attackedUnits.end(); i++)
    {
        if((*i)->unit == _unit)
            return (*i);
    }

    attackedUnit* attacked = new attackedUnit;
    attacked->unit = _unit;
    attacked->remainedCoolDown = 0;

    attackedUnits.push_back( attacked );

    return attacked;
}

//---------------------------------------------------------------------------------------------------------------------------------
