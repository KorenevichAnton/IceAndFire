#include "HumanKnightMobAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
HumanKnightMobAI::HumanKnightMobAI():
UnitAI(),
initialize(false),
waitAnimation(0),
blockChance(0)
{
    typeUnitAI = HUMAN_KNIGHT_MOB_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
HumanKnightMobAI::~HumanKnightMobAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
HumanKnightMobAI* HumanKnightMobAI::createTS(BattleModel* _battleModel)
{
    HumanKnightMobAI* pRet = new HumanKnightMobAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HumanKnightMobAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */
    
    blockChance = _unitAI->getAdditionalParameter(1);   //chance of block bullet damage

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HumanKnightMobAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);

    
    //update actions cooldowns
    this->updateActionsCooldown(_unitAI, _dt);
    //
    
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
void HumanKnightMobAI::damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg)
{

    if(_unitAI)
    {
        if(_attacker->getType() == AUM_TYPE_BULLET)
        {
            if( _attacker->getBehavior()->getTypeUnitAI() != BULLET_BULLET_AI &&
               _attacker->getBehavior()->getTypeUnitAI() != TRAP_BULLET_AI )
            {
                if(rand()%100 < blockChance)
                {
                    _realDmg = 0;
                    pBattleModel->showMessage(_unitAI, "Blocked!", _attack->dmgColor, ccp(_unitAI->getPosition().x, _unitAI->getPosition().y + 200));
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
