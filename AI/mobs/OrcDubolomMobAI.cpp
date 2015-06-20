#include "OrcDubolomMobAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "BufAll.h"
#include "cmdAttack.h"
#include "HUDScreen.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
OrcDubolomMobAI::OrcDubolomMobAI():
UnitAI(),
waitAnimation(0),
initialize(false)
{
    typeUnitAI = ORC_DUBOLOM_MOB_AI;
}

//---------------------------------------------------------------------------------------------------------------------------------
OrcDubolomMobAI::~OrcDubolomMobAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
OrcDubolomMobAI* OrcDubolomMobAI::createTS(BattleModel* _battleModel)
{
    OrcDubolomMobAI* pRet = new OrcDubolomMobAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void OrcDubolomMobAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void OrcDubolomMobAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
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

        if(useful == 0)
        {
            UnitAttack* att = _unitAI->getAttack(priority);

            _unitAI->setState( att->animName );
            waitAnimation = att->damageTime;


            if(att->type == AT_ENEMY)
            {
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, _unitAI->getCurAttackTarget(), att, att->damageTime);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();

                //use cooldowns
                this->useActionCooldown(_unitAI, priority);
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

                _unitAI->setState("move1");

                //calculate a new position
                float path = _unitAI->getMovingSpeed() * _dt / ccpDistance( _unitAI->getMoveTarget(), _unitAI->getPosition() );
                _unitAI->setPosition( ccpAdd( _unitAI->getPosition(),
                                             ccpMult(ccpSub( _unitAI->getMoveTarget(), _unitAI->getPosition() ), path) ) );

                //check collision for push enemies
                CCObject* obj;
                CCARRAY_FOREACH(_allUnits, obj)
                {
                    AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
                    if( unit->isAlive() &&
                        unit->getType() == AUM_TYPE_MOB &&
                        unit->getTeam() != _unitAI->getTeam() &&
                        unit->getLine() == _unitAI->getLine() )
                    {
                        
                        if(fabs(_unitAI->getPosition().x - unit->getPosition().x) <= MOB_COLLISION_DISTANCE_X)
                        {
                            if(!unit->issetBuf(BUF_PUSH))
                            {
                                CCObject* objBuf;
                                CCARRAY_FOREACH(_unitAI->_bufs, objBuf)
                                {
                                    Buf* buf = (Buf*)objBuf;
                                    if(buf->getKind() == BUF_SEND)
                                    {
                                        if(buf->getType() == BUF_PUSH)
                                        {
                                            BufPush* bufPush = dynamic_cast<BufPush*>(buf);
                                            bufPush->setSpeed(_unitAI->getMovingSpeed());
                                            bufPush->setDistance(_unitAI->getMovingSpeed());
                                            unit->addBuf( bufPush->copy() );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //
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
bool OrcDubolomMobAI::canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
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
