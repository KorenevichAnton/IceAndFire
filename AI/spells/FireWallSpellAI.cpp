#include "FireWallSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
FireWallSpellAI::FireWallSpellAI():
UnitAI(),
typeUnitAI(FIRE_WALL_SPELL_AI),
initialize(false),
remainedCoolDown(0)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
FireWallSpellAI::~FireWallSpellAI()
{
}
//---------------------------------------------------------------------------------------------------------------------------------
void FireWallSpellAI::setPosition(cocos2d::CCPoint _position)
{
    position = _position;
}
//---------------------------------------------------------------------------------------------------------------------------------
cocos2d::CCPoint FireWallSpellAI::getPosition()
{
    return position;
}
//---------------------------------------------------------------------------------------------------------------------------------
FireWallSpellAI* FireWallSpellAI::createTS(BattleModel* _battleModel)
{
    FireWallSpellAI* pRet = new FireWallSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;
    
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void FireWallSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a set spell position and initialize default parameters
     */
    
    if(_unitAI->isAlive())
    {
        float offset = 50;   //indentation from mob
        cocos2d::CCPoint pos(position.x, _unitAI->getPosition().y );  //positioning
        /*
         CCObject* obj = NULL;
         CCARRAY_FOREACH(_allUnits, obj)
         {
         AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
         if( unit->isAlive() &&
         unit->getType() == AUM_TYPE_MOB &&
         unit->getTeam() != _unitAI->getTeam() &&
         unit->getLine() == _unitAI->getLine() )
         {
         if(_unitAI->getTeam() == 1)
         {
         if(unit->getPosition().x < pos.x)
         pos.x = unit->getPosition().x;
         }
         else
         {
         if(unit->getPosition().x > pos.x)
         pos.x = unit->getPosition().x;
         }
         }
         }
         
         if(pos.x == 0 || pos.x == 1024)
         pos.x = 512;    //
         else
         {
         if(_unitAI->getTeam() == 1)
         pos.x -= offset;
         else
         pos.x += offset;
         }
         */
        
        if(_unitAI->getTeam() == 1)
            pos.x += offset;
        else
            pos.x -= offset;
        
        _unitAI->setPosition(pos);
    }
    
    
    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void FireWallSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);
    
    if(_unitAI->isAlive())
    {
        remainedCoolDown -= _dt;
        
        if(remainedCoolDown <= 0)
        {
            CCObject* obj = NULL;
            CCARRAY_FOREACH(_allUnits, obj)
            {
                AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
                int priority = 0;
                if( unit->isAlive() &&
                    unit->isMob() &&
                    unit->getTeam() != _unitAI->getTeam() &&
                    canAttack(_unitAI, unit, priority))
                {
                    cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                    pBattleModel->addCommand(cmdAtt);
                    cmdAtt->release();
                }
            }
            
            remainedCoolDown = _unitAI->getAttack()->damageTime;
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
bool FireWallSpellAI::canAttack(AnimatedUnitModel* attacker, AnimatedUnitModel* target, int& priority)
{
    if(target->isAlive() || isCanFinishUndead(attacker, target, priority))
    {
        //check all bufs - may be some of them could stop
        CCObject* obj = NULL;
        CCARRAY_FOREACH(attacker->getBufs(), obj)
        {
            Buf* buf = (Buf*) obj;
            if (((buf->getType() == BUF_PUSH)  ||
                 (buf->getType() == BUF_FREEZE)) &&
                (buf->getState() == BUF_ACTIVATED))
            {
                return false;
            }
        }
        
        //check all states - may be some of them could stop
        if( attacker->getState() == "death1" ||
           attacker->getState() == "death2" ||
           attacker->getState() == "death3" ||
           attacker->getState() == "death4" ||
           attacker->getState() == "death5" ||
           attacker->getState() == "damage1" )
        {
            return false;
        }
        
        //check on changing line
        if( attacker->getChangingLine() > 0 )
        {
            return  -1;
        }
        
        
        if(isBodyCircleInRange(attacker, target, priority))
        {
            if(isBodyPolygonInRange(attacker, target, priority))
            {
                if(target->isMob())
                    return true;
            }
        }
    }
    
    return false;
}
//---------------------------------------------------------------------------------------------------------------------------------
bool FireWallSpellAI::isBodyCircleInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority)
{
    UnitAttack*         action = (UnitAttack*)_attacker->_attacks->objectAtIndex(_priority);
    PhysicUnitModel*    body = _target->getPhysicalModel();
    
    cocos2d::CCPoint     actionOrigin = ccpAdd( _attacker->getPosition(), action->rangePoint );
    cocos2d::CCPoint     bodyOrigin = ccpAdd( _target->getPosition(), body->getCircleOrigin() );
    
    if(actionOrigin.x - bodyOrigin.x < action->range + body->getRadius())
        return true;
    
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool FireWallSpellAI::isBodyPolygonInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority)
{
    UnitAttack*         action = (UnitAttack*)_attacker->_attacks->objectAtIndex(_priority);
    PhysicUnitModel*    body = _target->getPhysicalModel();
    
    cocos2d::CCPoint    actionOrigin = ccpAdd( _attacker->getPosition(), action->rangePoint );
    cocos2d::CCPoint    bodyOrigin = ccpAdd( _target->getPosition(), body->getCircleOrigin() );
    
    //find range points on straight "actionOrigin -> bodyOrigin"
    cocos2d::CCPoint     vec = ccpSub(bodyOrigin, actionOrigin);
    float                vecLen = ccpDistance(actionOrigin, bodyOrigin);
    cocos2d::CCPoint     vecNorm = ccp(vec.x / vecLen, vec.y / vecLen);
    cocos2d::CCPoint     rangePointA = ccpAdd( ccpMult(vecNorm, action->range), actionOrigin);
    cocos2d::CCPoint     rangePointB = ccpAdd( ccpMult(vecNorm, -action->range), actionOrigin);
    
    //give the local coordinates of range point relative to target position
    cocos2d::CCPoint     pointA = ccpSub(rangePointA, _target->getPosition());
    cocos2d::CCPoint     pointB = ccpSub(rangePointB, _target->getPosition());
    
    //check on the intersection circle of attack and polygon
    bool left = false, right = false, top = false, down = false; //intersection condition
    for(vector<cocos2d::CCPoint>::const_iterator i = body->getPolygon().begin(); i < body->getPolygon().end(); i++)
    {
        if(pointA.x < (*i).x)
            left = true;
        else
            right = true;
        
        if(pointB.x < (*i).x)
            left = true;
        else
            right = true;
        
            down = true;
            top = true;
        
        //check entry
        if(left * right * top * down)
            return true;
    }
    
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
