#include "SceletonWarriorMobAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
SceletonWarriorMobAI::SceletonWarriorMobAI():
UnitAI(),
waitAnimation(0),
initialize(false),
acceleration(0)
{
    typeUnitAI = SCELETON_WARRIOR_MOB_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
SceletonWarriorMobAI::~SceletonWarriorMobAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
SceletonWarriorMobAI* SceletonWarriorMobAI::createTS(BattleModel* _battleModel)
{
    SceletonWarriorMobAI* pRet = new SceletonWarriorMobAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void SceletonWarriorMobAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    acceleration = _unitAI->getAdditionalParameter(1);    //acceleration of movement in percent

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void SceletonWarriorMobAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);


    this->updateActionsCooldown(_unitAI, _dt);


    //update of delay for playing animations
    if(waitAnimation)
    {
        waitAnimation -= _dt;
        if(waitAnimation <= 0)
            waitAnimation = 0;
    }
    //
    
    if(_unitAI->isAlive() && !waitAnimation)
    {
        int priority = 0;
        float useful = canAction(_unitAI, _allUnits, priority);   //check the possibility of the action

        //for escape from its landing zone (for team 1)
        if( _unitAI->getTeam() == 1 &&
            _unitAI->getPosition().x < (Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 2 &&
            canMove(_unitAI, _allUnits) )
        {
            useful = -1;
        }
        //

        //for escape from its landing zone (for team 2)
        if( _unitAI->getTeam() == 2 &&
            _unitAI->getPosition().x > Settings::sharedInstance()->getBattleFieldRect().size.width - (Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 2 &&
               canMove(_unitAI, _allUnits) )
        {
            useful = -1;
        }
        //

        //check unit need move or wait while cooldown of friendly action
        if(_unitAI->getAttack(priority)->type == AT_FRIENDLY && useful > 0)
        {
            bool wait = false;
            CCObject* obj;
            CCARRAY_FOREACH(_unitAI->_attacks, obj)
            {
                UnitAttack* uAtt = (UnitAttack*)obj;
                if(uAtt->type == AT_ENEMY && getEnemy(_unitAI, _allUnits, uAtt->priority))
                {
                    wait = true;
                    break;
                }
            }
            
            if(!wait)
                useful = -1;
        }
        //
        
        if(useful >= 0)
        {
            if(useful == 0)
            {

                
                //set direction on attack target
                if(_unitAI->getCurAttackTarget() == _unitAI)
                    _unitAI->setLookMode( (_unitAI->getTeam() == 1) ? AUM_LOOK_LEFT_MODE : AUM_LOOK_RIGHT_MODE );
                else
                {
                    if(_unitAI->getPosition().x < _unitAI->getCurAttackTarget()->getPosition().x)
                        _unitAI->setLookMode(AUM_LOOK_LEFT_MODE);
                    else
                        _unitAI->setLookMode(AUM_LOOK_RIGHT_MODE);
                }
                //
                UnitAttack* att = _unitAI->getAttack(priority);

                _unitAI->setState( att->animName );
                waitAnimation = att->damageTime;


                if(att->type == AT_ENEMY)
                {
                    if(att->subtype == "melee")
                    {
                        cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, _unitAI->getCurAttackTarget(), att, att->damageTime);
                        pBattleModel->addCommand(cmdAtt);
                        cmdAtt->release();

                        //use cooldowns
                        this->useActionCooldown(_unitAI, priority);
                    }
                }
            }
            else
                _unitAI->setState("wait1");  //waiting cooldowns
        }
        else
        {
            combatOrdering(_unitAI, _allUnits, _dt);    //for new gameplay conception
            
            if(canMove(_unitAI, _allUnits))
            {
                //set direction on move target
                if(_unitAI->getPosition().x < _unitAI->getMoveTarget().x)
                    _unitAI->setLookMode(AUM_LOOK_LEFT_MODE);
                else
                    _unitAI->setLookMode(AUM_LOOK_RIGHT_MODE);
                //

                _unitAI->setState("move1");

                //calculate a new position
                float path = _unitAI->getMovingSpeed() * _dt / ccpDistance( _unitAI->getMoveTarget(), _unitAI->getPosition() );
                _unitAI->setPosition( ccpAdd( _unitAI->getPosition(),
                                             ccpMult(ccpSub( _unitAI->getMoveTarget(), _unitAI->getPosition() ), path) ) );   
            }
            else
            {
                if(_unitAI->getChangingLine() == 0)
                    _unitAI->setState("wait1");
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void SceletonWarriorMobAI::damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg)
{

    if(_unitAI)
    {
        //increase movement speed
        _unitAI->setMovingSpeed( _unitAI->getMovingSpeed() + _unitAI->getMovingSpeed() * acceleration * 0.01 );

        pBattleModel->showMessage(_unitAI, "Accelerate!", _attack->dmgColor, ccp(_unitAI->getPosition().x, _unitAI->getPosition().y + 200));
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
