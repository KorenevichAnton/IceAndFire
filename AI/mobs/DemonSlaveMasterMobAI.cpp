#include "DemonSlaveMasterMobAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAddUnit.h"
#include "cmdAttack.h"
#include "BulletFactory.h"
#include "MobFactory.h"
#include "HUDScreen.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
DemonSlaveMasterMobAI::DemonSlaveMasterMobAI():
UnitAI(),
initialize(false),
waitAnimation(0),
fireWallLifeTime(0),
specialAnimationDuration(0)
{
    typeUnitAI = DEMON_SLAVEMASTER_MOB_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
DemonSlaveMasterMobAI::~DemonSlaveMasterMobAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
DemonSlaveMasterMobAI* DemonSlaveMasterMobAI::createTS(BattleModel* _battleModel)
{
    DemonSlaveMasterMobAI* pRet = new DemonSlaveMasterMobAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void DemonSlaveMasterMobAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    waitAnimation = 0.5;  //waiting time for playing "beginSpecialMove1" and "endSpecialMove1"
    specialAnimationDuration = _unitAI->getAdditionalParameter(1);
    //fireWallLifeTime = 10;  //life time of fireWall bullet
    fireWallLifeTime = _unitAI->getAdditionalParameter(2);
    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void DemonSlaveMasterMobAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
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
            if(canAttack(_unitAI, _unitAI->getCurAttackTarget(), priority) || _unitAI->getAttack(priority)->type == AT_FRIENDLY)
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
                if(useful == 0)
                {
                    if(_unitAI->getState() == "specialMove1" || _unitAI->getState() == "beginSpecialMove1")
                    {
                        _unitAI->setState("endSpecialMove1");
                        waitAnimation = specialAnimationDuration;
                    }
                    else
                    {
                        UnitAttack* att = _unitAI->getAttack(priority);
                        
                        _unitAI->setState( att->animName );
                        waitAnimation = att->damageTime;
                        
                        if(att->type == AT_ENEMY)
                        {
                            if(att->subtype == "ranged")
                            {
                                AnimatedUnitModel* bullet = BulletFactory::createTS(att->generatedUnit, pBattleModel);
                                if(bullet)
                                {
                                    bullet->pBattleModel = pBattleModel;
                                    bullet->setInvoker(_unitAI);
                                    bullet->setTeam(_unitAI->getTeam());
                                    bullet->setLine(_unitAI->getLine());
                                    bullet->setLookMode(_unitAI->getLookMode());
                                    //set position
                                    if(bullet->getMovingSpeed() > 0)
                                        bullet->setPosition(_unitAI->getPosition());
                                    else
                                    {
                                        float offset = (_unitAI->getPosition().x - _unitAI->getCurAttackTarget()->getPosition().x) / 2;
                                        
                                        bullet->setPosition( ccp(_unitAI->getCurAttackTarget()->getPosition().x + offset,
                                                                 _unitAI->getCurAttackTarget()->getPosition().y) );
                                        
                                        bullet->setLifeTime(fireWallLifeTime);
                                    }
                                    
                                    //set attack stats from invoker
                                    bullet->getAttack()->damage = att->damage;
                                    bullet->getAttack()->damageDelta = att->damageDelta;
                                    bullet->getAttack()->damageTime = 0;
                                    bullet->getAttack()->criticalChance = att->criticalChance;
                                    bullet->getAttack()->criticalPower = att->criticalPower;
                                    bullet->getAttack()->schoolName = att->schoolName;
                                    
                                    //set attack bufs from invoker
                                    CCObject* objBuf;
                                    CCARRAY_FOREACH(att->_bufs, objBuf)
                                    {
                                        Buf* buf = (Buf*)objBuf;
                                        if(buf->getKind() == BUF_SEND)
                                        {
                                            Buf* newcopy = buf->copy();
                                            bullet->getAttack()->_bufs->addObject(newcopy);
                                            newcopy->release();
                                        }
                                    }
                                    
                                    cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, bullet, att->damageTime);
                                    pBattleModel->addCommand(add);
                                    add->release();
                                    
                                    bullet->release();
                                    
                                    this->useActionCooldown(_unitAI, priority);
                                }
                            }
                        }
                        
                        if(att->type == AT_FRIENDLY)
                        {
                            if(att->subtype == "summon")
                            {
                                AnimatedUnitModel* mob = MobFactory::createTS(att->generatedUnit ,pBattleModel);
                                
                                if(mob)
                                {
                                    mob->setInvoker(_unitAI);
                                    mob->setTeam(_unitAI->getTeam());
                                    mob->setLookMode(_unitAI->getLookMode());
                                    mob->setLine(_unitAI->getLine());
                                    mob->setPosition(_unitAI->getPosition());
                                    mob->setMoveTarget(_unitAI->getMoveTarget());
                                    
                                    cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, mob, att->damageTime);
                                    pBattleModel->addCommand(add);
                                    add->release();
                                    
                                    mob->release();
                                    
                                    this->useActionCooldown(_unitAI, priority);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(_unitAI->getState() == "specialMove1")
                    {
                        _unitAI->setState("endSpecialMove1");
                        waitAnimation = specialAnimationDuration;
                    }
                    else
                        _unitAI->setState("wait1");  //waiting cooldowns
                }
            }
            else
            {
                if(canMove(_unitAI, _allUnits))
                {
                    if(_unitAI->getState() != "specialMove1" &&
                       _unitAI->getState() != "beginSpecialMove1")
                    {
                        _unitAI->setState("beginSpecialMove1");
                        waitAnimation = specialAnimationDuration;
                    }
                    else
                    {
                        _unitAI->setState("specialMove1");
                    
                        //calculate a new position for move to AttackTarget
                        float path = _unitAI->getMovingSpeed() * _dt / ccpDistance( _unitAI->getCurAttackTarget()->getPosition(), _unitAI->getPosition() );
                        _unitAI->setPosition( ccpAdd( _unitAI->getPosition(),
                                                             ccpMult(ccpSub( _unitAI->getCurAttackTarget()->getPosition(), _unitAI->getPosition() ), path) ) );
                    
                        
                    }
                }
                else
                {
                    if(_unitAI->getState() == "specialMove1")
                    {
                        _unitAI->setState("endSpecialMove1");
                        waitAnimation = specialAnimationDuration;
                    }
                    else
                        _unitAI->setState("wait1");
                }
            }
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
                
                if(_unitAI->getState() != "specialMove1" &&
                   _unitAI->getState() != "beginSpecialMove1")
                {
                    _unitAI->setState("beginSpecialMove1");
                    waitAnimation = specialAnimationDuration;
                }
                else
                {
                    _unitAI->setState("specialMove1");
                    
                    //calculate a new position for move to MoveTarget
                    float path = _unitAI->getMovingSpeed() * _dt / ccpDistance( _unitAI->getMoveTarget(), _unitAI->getPosition() );
                    _unitAI->setPosition( ccpAdd( _unitAI->getPosition(),
                                                             ccpMult(ccpSub( _unitAI->getMoveTarget(), _unitAI->getPosition() ), path) ) );
                    
                    
                }
            }
            else
            {
                if(_unitAI->getState() == "specialMove1")
                {
                    _unitAI->setState("endSpecialMove1");
                    waitAnimation = specialAnimationDuration;
                }
                else
                {
                    if(_unitAI->getChangingLine() == 0)
                        _unitAI->setState("wait1");
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* DemonSlaveMasterMobAI::getEnemy(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority)
{
    AnimatedUnitModel* target = NULL;
    float minDistance = 0;
    
    CCObject* obj;
    CCARRAY_FOREACH(_allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
        if( ( unit->isAlive() ||
             isCanFinishUndead(_unitAI, unit, _priority) ) &&
           unit->getType() == AUM_TYPE_MOB &&
           unit->getTeam() != _unitAI->getTeam() &&
           unit->getLine() == _unitAI->getLine() )
        {
            /* flying demons not target for melee */
            if(!( (_unitAI->getSort() == DEXTERITY_MELEE_MOB_AUM ||
                   _unitAI->getSort() == STRENGTH_MELEE_MOB_AUM) &&
                 (unit->getBehavior()->getTypeUnitAI() == DEMON_SMOKE_MOB_AI ||
                  unit->getBehavior()->getTypeUnitAI() == DEMON_FIRESPITTER_MOB_AI ||
                  unit->getBehavior()->getTypeUnitAI() == DEMON_SLAVEMASTER_MOB_AI) &&
                 (unit->getState() == "beginSpecialMove1" ||
                  unit->getState() == "specialMove1" ||
                  unit->getState() == "endSpecialMove1") ))
            {
                float dis = ccpDistance(unit->getPosition(), _unitAI->getPosition());
                
                if(target)
                {
                    if( unit->getSort() == PHYS_RANGE_MOB_AUM ||
                       unit->getSort() == MAGE_RANGE_MOB_AUM )
                    {
                        if( target->getSort() != PHYS_RANGE_MOB_AUM &&
                           target->getSort() != PHYS_RANGE_MOB_AUM )
                        {
                            target = unit;
                            minDistance = dis;
                        }
                        else if(dis < minDistance)
                        {
                            target = unit;
                            minDistance = dis;
                        }
                    }
                    else
                    {
                        if( target->getSort() != PHYS_RANGE_MOB_AUM &&
                           target->getSort() != PHYS_RANGE_MOB_AUM &&
                           dis < minDistance )
                        {
                            target = unit;
                            minDistance = dis;
                        }
                    }
                }
                else
                {
                    target = unit;
                    minDistance = dis;
                }
            }
        }
    }
    
    return target;
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* DemonSlaveMasterMobAI::getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority)
{
    AnimatedUnitModel* target = _unitAI;
    
    return target;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool DemonSlaveMasterMobAI::canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    //check capture checkpoint
    if(checkCheckpoint(_unitAI))
    {
        if(this->tryCaptureCheckpoint(_unitAI))
            return false;
    }
    
    //check all bufs - may be some of them could stop move
    CCObject* obj;
    CCARRAY_FOREACH(_unitAI->_bufs, obj)
    {
        Buf* buf = (Buf*)obj;
        if( buf->getState() == BUF_ACTIVATED &&
           ( buf->getType() == BUF_FREEZE ||
            buf->getType() == BUF_PUSH ) )
        {
            return false;
        }
    }
    //
    
    //check state - may be them could stop move
    if( _unitAI->getState() == "death1" ||
        _unitAI->getState() == "death2" ||
        _unitAI->getState() == "death3" ||
        _unitAI->getState() == "death4" ||
        _unitAI->getState() == "death5" ||
        _unitAI->getState() == "damage1" )
    {
        return false;
    }
    //
    
    if(Settings::sharedInstance()->getGameMode() == GM_SINGLEPLAYER)
    {
        if( _unitAI->getType() == AUM_TYPE_MOB &&
           _unitAI->getTeam() == 1 )
        {
            if(_unitAI->getPosition().x >= HUDScreen::screenWidth / 2)
                return false;
        }
    }
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void DemonSlaveMasterMobAI::damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg)
{
    if(_unitAI->isAlive())
    {
        if(_attack->schoolName == "fire")
        {
            _realDmg = 0;
            pBattleModel->showMessage(_unitAI, "Fire resistance!", _attack->dmgColor, ccp(_unitAI->getPosition().x, _unitAI->getPosition().y + 200));

        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
