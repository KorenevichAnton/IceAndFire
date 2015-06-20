#include "ZombieDarkKnightMobAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "MyBattleScene.h"
#include "CCheckpoint.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
ZombieDarkKnightMobAI::ZombieDarkKnightMobAI():
UnitAI(),
waitAnimation(0),
initialize(false)
{
    typeUnitAI = ZOMBIE_DARK_KNIGHT_MOB_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
ZombieDarkKnightMobAI::~ZombieDarkKnightMobAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
ZombieDarkKnightMobAI* ZombieDarkKnightMobAI::createTS(BattleModel* _battleModel)
{
    ZombieDarkKnightMobAI* pRet = new ZombieDarkKnightMobAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void ZombieDarkKnightMobAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void ZombieDarkKnightMobAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
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
void ZombieDarkKnightMobAI::damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg)
{
    if(_unitAI->isAlive())
    {
        if( _realDmg >= _unitAI->getHp() &&
           _attack->schoolName != "fire" )
        {
            //activate resurrection
            CCObject* obj;
            CCARRAY_FOREACH(_unitAI->_bufs, obj)
            {
                Buf* buf = (Buf*)obj;
                if(buf->getKind() == BUF_SEND)
                {
                    if(buf->getType() == BUF_RESURRECTION)
                    {
                        MyBattleScene* bScene = MyBattleScene::pSelf;
                        std::vector<CCheckpoint*> checkpoints = bScene->getCheckpoints();
                        CCheckpoint* lastCheckpoint = NULL;
                        for(int j = 0; j < 5; j++)
                        {
                            if(checkpoints[j]->getTeam() == _unitAI->getTeam())
                            {
                                if(!lastCheckpoint || checkpoints[j]->getPosition().x < lastCheckpoint->getPositionX())
                                    lastCheckpoint = checkpoints[j];
                            }
                        }
                        if(lastCheckpoint)
                        {
                            if(_unitAI->getTeam() == TF_BLUE && lastCheckpoint->getPosition().x >= _unitAI->getPosition().x)
                            {
                                _unitAI->addBuf( buf );
                            }else if(_unitAI->getTeam() == TF_RED && lastCheckpoint->getPosition().x <= _unitAI->getPosition().x)
                            {
                                _unitAI->addBuf( buf );
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(_attack->schoolName == "fire")
        {
            //disactivate resurrection
            CCObject* obj;
            CCARRAY_FOREACH(_unitAI->_bufs, obj)
            {
                Buf* buf = (Buf*)obj;
                if(buf->getKind() == BUF_SELF)
                {
                    if(buf->getType() == BUF_RESURRECTION)
                        buf->disactivate();
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------