#include "UnitAI.h"
#include "AnimatedUnitModel.h"
#include "BattleModel.h"
#include "Settings.h"
#include "Buf.h"

#include "Utils.h"
#include "HUDScreen.h"
#include "cmdAttack.h"

#include "Settings.h"
#include "cmdRemoveUnit.h"

#include "Card.h"
#include "HeroAI.h"
#include "MobFactory.h"
#include "Utils.h"

#include "UnitActions.h"

#include "Buf.h"

#include "MyBattleScene.h"

using namespace cocos2d;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
UnitAI::UnitAI():
typeUnitAI(DEFAULT_UNIT_AI),
pBattleModel(NULL)
{
    _settings = Settings::sharedInstance();
}

//---------------------------------------------------------------------------------------------------------------------------------
UnitAI::~UnitAI()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::tryCaptureCheckpoint(AnimatedUnitModel* _unit)
{
    vector<CCheckpoint*> checkpoints = MyBattleScene::pSelf->getCheckpoints();
    for(vector<CCheckpoint*>::iterator i = checkpoints.begin(); i < checkpoints.end(); i++)
    {
        if((*i)->checkZoneCapture(_unit))
        {
            (*i)->addUnit(_unit);
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::checkCheckpoint(AnimatedUnitModel* _unit)
{
    vector<CCheckpoint*> checkpoints = MyBattleScene::pSelf->getCheckpoints();
    for (vector<CCheckpoint*>::iterator i = checkpoints.begin(); i < checkpoints.end(); i++)
    {
        if((*i)->checkCapture(_unit) && (*i)->checkZoneCapture(_unit))
            return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::canAttack(AnimatedUnitModel* attacker, AnimatedUnitModel* target, int& priority)
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
float UnitAI::canAction(AnimatedUnitModel* attacker, const cocos2d::CCArray* allUnits, int& priority)
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
            return -1;
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
        return -1;
    }
    
    //check on changing line
    if( attacker->getChangingLine() > 0 )
    {
        return  -1;
    }
    
    
    AnimatedUnitModel* bestTarget = NULL;
    float bestCoolDown = 9999;
    float bestPriority = -1;
    
    for(int i = 0; i < attacker->_attacks->count(); i++)
    {
        priority = i;   //send number attack by priority
        
        AnimatedUnitModel*  target = NULL;
        UnitAttack* attModel = (UnitAttack*)attacker->_attacks->objectAtIndex(i);
        
        if(attModel->type == AT_ENEMY)
            target = getEnemy(attacker, allUnits, priority);
        
        if(attModel->type == AT_FRIENDLY)
            target = getFriendly(attacker, allUnits, priority);
        
        if(target)
        {
            if(attModel->remainedColdDown == 0 &&
               attModel->remainedActionColdDown == 0)   //if can action
            {
                attacker->setCurAttackTarget(target);
                return 0;
            }
            else                                //if may action after delay
            {
                if(attModel->remainedColdDown < bestCoolDown)
                {
                    bestTarget = target;
                    bestCoolDown = attModel->remainedColdDown;
                    bestPriority = priority;
                }
            }
        }
    }
    
    //if may action only after delay
    if(bestPriority >= 0)
    {
        attacker->setCurAttackTarget(bestTarget);
        priority = bestPriority;
        
        UnitAttack* action = attacker->getAttack(priority);
        if(action->remainedActionColdDown > bestCoolDown)
            bestCoolDown = action->remainedActionColdDown;
        
        return bestCoolDown;
    }
    
    return  -1;
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* UnitAI::getEnemy(AnimatedUnitModel* _unitAI,const cocos2d::CCArray* allUnits, int priority)
{
    CCObject* obj;
    AnimatedUnitModel* target = NULL;
    float minDistance = 0;
    
    CCARRAY_FOREACH(allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if ( (unit->isAlive() || isCanFinishUndead(_unitAI, unit, priority)) &&
            unit->getType() == AUM_TYPE_MOB &&
            unit->getTeam() != _unitAI->getTeam())
        {
            if( ( (_unitAI->getSort() == DEXTERITY_MELEE_MOB_AUM || _unitAI->getSort() == STRENGTH_MELEE_MOB_AUM) &&
                  (abs(unit->getLine() - _unitAI->getLine()) <= 2) ) ||
                ( (_unitAI->getSort() == PHYS_RANGE_MOB_AUM || _unitAI->getSort() == MAGE_RANGE_MOB_AUM) &&
                  (unit->getLine() == _unitAI->getLine()) ) )
            {
                if(isBodyCircleInRange(_unitAI, unit, priority))
                {
                    if(isBodyPolygonInRange(_unitAI, unit, priority))
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
                            //find the nearest target
                            float dis = ccpDistance(unit->getPosition(), _unitAI->getPosition());
                            if(!target || dis < minDistance)
                            {
                                target = unit;
                                minDistance = dis;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return target;
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* UnitAI::getFriendly(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits, int priority)
{
    CCObject* obj;
    AnimatedUnitModel* target = NULL;
    
    CCARRAY_FOREACH(allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        if (unit->isAlive() &&
            unit->isMob() &&
            unit->getTeam() == unitAI->getTeam() &&
            unit != unitAI /*&&
            unit->getLine() == unitAI->getLine()*/)
        {
            if(isBodyCircleInRange(unitAI, unit, priority))
            {
                if(isBodyPolygonInRange(unitAI, unit, priority))
                {
                    //find nobody
                    
                    return false;
                }
            }
        }
    }
    return target;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::isBodyCircleInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority)
{
    UnitAttack*         action = (UnitAttack*)_attacker->_attacks->objectAtIndex(_priority);
    PhysicUnitModel*    body = _target->getPhysicalModel();
    
    cocos2d::CCPoint     actionOrigin = ccpAdd( _attacker->getPosition(), action->rangePoint );
    cocos2d::CCPoint     bodyOrigin = ccpAdd( _target->getPosition(), body->getCircleOrigin() );
    
    if(ccpDistance(actionOrigin, bodyOrigin) < action->range + body->getRadius())
        return true;
    
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::isBodyPolygonInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority)
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
        
        if(pointA.y < (*i).y)
            down = true;
        else
            top = true;
        
        if(pointB.y < (*i).y)
            down = true;
        else
            top = true;
        
        //check entry
        if(left * right * top * down)
            return true;
    }
    
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::isCanFinishUndead(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority)
{
    if( ( _attacker->getAttack(_priority)->schoolName == "fire" ) &&
       ( !_target->isAlive() ) && (_target->getType() != AUM_TYPE_CHAR)
       &&
       ( _target->getBehavior()->getTypeUnitAI() == ZOMBIE_MOB_AI ||
        _target->getBehavior()->getTypeUnitAI() == ZOMBIE_CROSSBOW_MOB_AI ||
        _target->getBehavior()->getTypeUnitAI() == ZOMBIE_DARK_KNIGHT_MOB_AI ||
        _target->getBehavior()->getTypeUnitAI() == ZOMBIE_SHAMAN_MOB_AI ) )
    {
        CCObject* obj;
        CCARRAY_FOREACH(_target->_bufs, obj)
        {
            Buf* buf = (Buf*)obj;
            if(buf->getState() == BUF_ACTIVATED)
            {
                if(buf->getType() == BUF_RESURRECTION)
                    return true;
            }
        }
    }
    
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool UnitAI::canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allunits)
{   
    //check all bufs - may be some of them could stop
    CCObject* obj = NULL;
    CCARRAY_FOREACH(_unitAI->getBufs(), obj)
    {
        Buf* buf = (Buf*) obj;
        if (((buf->getType() == BUF_PUSH)  ||
             (buf->getType() == BUF_FREEZE)) &&
            (buf->getState() == BUF_ACTIVATED))
        {
            return false;
        }
    }
    //
    
    //check all states - may be some of them could stop
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
    
    /* MAY BE IN NEW GAMEPLAY CONCEPTION THIS DON'T NEED
    if(Settings::sharedInstance()->getGameMode() == GM_SINGLEPLAYER)
    {
        if( _unitAI->getType() == AUM_TYPE_MOB &&
           _unitAI->getTeam() == 1 )
        {
            if(_unitAI->getPosition().x >= HUDScreen::screenWidth / 2)
                return false;
        }
    }
    */
    
    //check capture checkpoint  
    if(checkCheckpoint(_unitAI))
    {
        if(this->tryCaptureCheckpoint(_unitAI))
        {
            bool targetForFighterFound = false;   //for mob fighter zone
            
            if(_unitAI->getSort() == DEXTERITY_MELEE_MOB_AUM)
            {
                CCObject* obj = NULL;
                CCARRAY_FOREACH(_allunits, obj)
                {
                    AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
                    
                    if( unit->isAlive() &&
                        unit->isMob() &&
                        _unitAI->getTeam() != unit->getTeam() &&
                        /*_unitAI->getLine() == unit->getLine() &&*/
                        ( (_unitAI->getTeam() == 1) ?
                          (_unitAI->getPosition().x + MOB_FIGHTER_VISION_DISTANCE >= unit->getPosition().x) :
                          (_unitAI->getPosition().x - MOB_FIGHTER_VISION_DISTANCE <= unit->getPosition().x) ) )
                    {
                        targetForFighterFound = true;
                        break;
                    }
                }
            }
            
            if(_unitAI->getSort() != DEXTERITY_MELEE_MOB_AUM || !targetForFighterFound)
                return false;
        }
    }
    //
    
    
    CCPoint unitAiPos = _unitAI->getPosition();
    
    CCARRAY_FOREACH(_allunits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
        
        CCPoint unitPos = unit->getPosition();
        
        if( unit->isAlive() &&
           unit->getType() == AUM_TYPE_MOB &&
           unit != _unitAI )
        {
            if(fabs(unitAiPos.x - unitPos.x) <= MOB_COLLISION_DISTANCE_X)
            {
                if(unit->getTeam() != _unitAI->getTeam())
                {
                    return false;
                }
                else
                {
                    if( ((_unitAI->getTeam() == 1) ? (unitAiPos.x < unitPos.x) : (unitAiPos.x > unitPos.x)) &&
                        ( (unit->getLine() == _unitAI->getLine()) ||
                          ( (unit->getChangingLine() != 0 || _unitAI->getChangingLine() != 0) &&
                            fabs(unitAiPos.y - unitPos.y) <= MOB_COLLISION_DISTANCE_Y ) ) &&
                         unit->getLifeTime() <= 0)
                    {
                        return false;
                    }
                }
            }
        }
    }
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void UnitAI::useActionCooldown(AnimatedUnitModel* _unitAI, int _priority)
{
    UnitAttack* action = _unitAI->getAttack(_priority);
    
    action->remainedColdDown = action->coldDown;
    
    CCObject* obj;
    CCARRAY_FOREACH(_unitAI->_attacks, obj)
    {
        UnitAttack* attModel = (UnitAttack*)obj;
        if(attModel->type == action->type)
            attModel->remainedActionColdDown = action->actionColdDown;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void UnitAI::updateActionsCooldown(AnimatedUnitModel* _unitAI, float _dt)
{
    CCObject* obj;
    CCARRAY_FOREACH(_unitAI->_attacks, obj)
    {
        UnitAttack* attModel = (UnitAttack*)obj;
        attModel->remainedColdDown -= _dt;
        if(attModel->remainedColdDown < _dt)
            attModel->remainedColdDown = 0;
        
        attModel->remainedActionColdDown -= _dt;
        if(attModel->remainedActionColdDown < _dt)
            attModel->remainedActionColdDown = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void UnitAI::combatOrdering(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    
/** determine the overall situation
 */
    int friendlies[3] = {0, 0, 0};  //count of friendly units on lines
    int enemies[3] = {0, 0, 0};     //count of enemy units on lines

    CCPoint nowAiPos = _unitAI->getPosition();
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(_allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
        
        CCPoint unitPos = unit->getPosition();
        
        if( unit != _unitAI &&
           unit->isAlive() &&
           unit->getType() == AUM_TYPE_MOB &&
           ( (_unitAI->getTeam() == 1) ?
            (unitPos.x > nowAiPos.x - MOB_BEFORE_VISION_DISTANCE && unitPos.x < nowAiPos.x + MOB_AFTER_VISION_DISTANCE) :
            (unitPos.x < nowAiPos.x + MOB_BEFORE_VISION_DISTANCE && unitPos.x > nowAiPos.x - MOB_AFTER_VISION_DISTANCE) ) )
        {
            int line = 0;
            
            if(unit->getChangingLine() == 0)
                line = unit->getLine();
            else
                line = unit->getChangingLine();
            
            //consider units
            if(line)
            {
                if(unit->getTeam() != _unitAI->getTeam())   //enemies
                {
                    if( (_unitAI->getTeam() == 1) ? (nowAiPos.x < unitPos.x) : (nowAiPos.x > unitPos.x) )   //consider only those enemies that are ahead
                        enemies[line - 1]++;
                }
                else                                        //friendlies
                    friendlies[line - 1]++;
            }
            //
        }
    }
//
    
    
/** logic of mobs behavior for changing line
 */
    int line = 0;
    
    if(_unitAI->getSort() == DEXTERITY_MELEE_MOB_AUM)
        line = combatLogicDexMelee(_unitAI, friendlies, enemies);
    else if(_unitAI->getSort() == PHYS_RANGE_MOB_AUM)
        line = combatLogicPhysRange(_unitAI, friendlies, enemies);
    else if(_unitAI->getSort() == STRENGTH_MELEE_MOB_AUM)
        line = combatLogicStrMelee(_unitAI, friendlies, enemies);
    else if(_unitAI->getSort() == MAGE_RANGE_MOB_AUM)
        line = combatLogicMageRange(_unitAI, friendlies, enemies);
    
    _unitAI->setChangingLine(line);
//
    
    
/** moving mobs for changing line
 */
    combatChangingLine(_unitAI, _dt);
//
    
}

//---------------------------------------------------------------------------------------------------------------------------------
int UnitAI::combatLogicDexMelee(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3])
{
    int line = 0;
    
    int strength[3] = {0, 0, 0};
    for(int i = 0; i < 3; i++)
        strength[i] = ((_friendlies[i] > 0) ? 1 : 0) - ((_enemies[i] > 0) ? 1 : 0);
    
    if(strength[1] == -1)
        line = 2;
    else
    {
        if(strength[0] == -1 && strength[2] == -1)
        {
            if(_friendlies[1] == 0)
                line = 2;
            else
            {
                if(_friendlies[0] == 0 && _friendlies[2] == 0)
                {
                    if(_unitAI->getLine() == 1)
                        line = 1;
                    else if(_unitAI->getLine() == 3)
                        line = 3;
                    else
                    {
                        if(_enemies[0] >= _enemies[2])
                            line = 1;
                        else
                            line = 3;
                    }
                }
                else if(_friendlies[0] == 0)
                    line = 1;
                else if(_friendlies[2] == 0)
                    line = 3;
            }
        }
        else if(strength[0] == -1)
            line = 1;
        else if(strength[2] == -1)
            line = 3;
        else
        {
/** additional logic (find vacant space in priority)
            // for consider themselves
            int f[3];
            for(int i = 0; i < 3; i++)
                f[i] = _friendlies[i];
            f[_unitAI->getTeam() - 1]++;
            //
        
            if( !(f[0] == f[1] == f[2]) )
            {
                int cmp = f[1];
                line = 2;
                
                if(f[0] < cmp)
                {
                    cmp = f[0];
                    line = 1;
                }
                
                if(f[2] < cmp)
                {
                    cmp = f[2];
                    line = 3;
                }
            }
*/
        }
    }
    
/*CCLOG( "COMBAT ORDERING:\n mob: %s\n line 3:  %d - %d = %d\n line 2:  %d - %d = %d\n line 1:  %d - %d = %d\n changing line: %d\n",
          _unitAI->getSkin().c_str(),
          _friendlies[2], _enemies[2], strength[2],
          _friendlies[1], _enemies[1], strength[1],
          _friendlies[0], _enemies[0], strength[0],
          line);
*/
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
int UnitAI::combatLogicPhysRange(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3])
{
    int line = 0;
    
    int strength[3] = {0, 0, 0};
    for(int i = 0; i < 3; i++)
        strength[i] = ((_friendlies[i] > 0) ? 1 : 0) - ((_enemies[i] > 0) ? 1 : 0);
    
    if(strength[1] == -1)
        line = 2;
    else
    {
        if(strength[0] == -1 && strength[2] == -1)
        {
            if(_friendlies[1] == 0)
                line = 2;
            else
            {
                if(_friendlies[0] == 0 && _friendlies[2] == 0)
                {
                    if(_unitAI->getLine() == 1)
                        line = 1;
                    else if(_unitAI->getLine() == 3)
                        line = 3;
                    else
                    {
                        if(_enemies[0] >= _enemies[2])
                            line = 1;
                        else
                            line = 3;
                    }
                }
                else if(_friendlies[0] == 0)
                    line = 1;
                else if(_friendlies[2] == 0)
                    line = 3;
            }
        }
        else if(strength[0] == -1)
            line = 1;
        else if(strength[2] == -1)
            line = 3;
        else
        {
/** additional logic (find target in priority)
            int cmp = 0;

            if(_enemies[1] > cmp)
            {
                cmp = _enemies[1];
                line = 2;
            }
            
            if(_enemies[0] > cmp)
            {
                cmp = _enemies[0];
                line = 1;
            }
            
            if(_enemies[2] > cmp)
            {
                cmp = _enemies[2];
                line = 3;
            }
*/
        }
    }
/*
    CCLOG( "COMBAT ORDERING:\n mob: %s\n line 3:  %d - %d = %d\n line 2:  %d - %d = %d\n line 1:  %d - %d = %d\n changing line: %d\n",
          _unitAI->getSkin().c_str(),
          _friendlies[2], _enemies[2], strength[2],
          _friendlies[1], _enemies[1], strength[1],
          _friendlies[0], _enemies[0], strength[0],
          line);
*/
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
int UnitAI::combatLogicStrMelee(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3])
{
    int line = 0;
    
    int strength[3] = {0, 0, 0};
    for(int i = 0; i < 3; i++)
        strength[i] = ((_friendlies[i] > 0) ? 1 : 0) - ((_enemies[i] > 0) ? 1 : 0);
    
    if(strength[1] == -1)
        line = 2;
    else
    {
        if(strength[0] == -1 && strength[2] == -1)
        {
            if(_friendlies[1] == 0)
                line = 2;
            else
            {
                if(_friendlies[0] == 0 && _friendlies[2] == 0)
                {
                    if(_unitAI->getLine() == 1)
                        line = 1;
                    else if(_unitAI->getLine() == 3)
                        line = 3;
                    else
                    {
                        if(_enemies[0] >= _enemies[2])
                            line = 1;
                        else
                            line = 3;
                    }
                }
                else if(_friendlies[0] == 0)
                    line = 1;
                else if(_friendlies[2] == 0)
                    line = 3;
            }
        }
        else if(strength[0] == -1)
            line = 1;
        else if(strength[2] == -1)
            line = 3;
        else
        {
/** additional logic (find vacant space in priority)
            // for consider themselves
            int f[3];
            for(int i = 0; i < 3; i++)
                f[i] = _friendlies[i];
            f[_unitAI->getTeam() - 1]++;
            //
            
            if( !(f[0] == f[1] == f[2]) )
            {
                int cmp = f[1];
                line = 2;
                
                if(f[0] < cmp)
                {
                    cmp = f[0];
                    line = 1;
                }
                
                if(f[2] < cmp)
                {
                    cmp = f[2];
                    line = 3;
                }
            }
*/
        }
    }
/*
    CCLOG( "COMBAT ORDERING:\n mob: %s\n line 3:  %d - %d = %d\n line 2:  %d - %d = %d\n line 1:  %d - %d = %d\n changing line: %d\n",
          _unitAI->getSkin().c_str(),
          _friendlies[2], _enemies[2], strength[2],
          _friendlies[1], _enemies[1], strength[1],
          _friendlies[0], _enemies[0], strength[0],
          line);
*/
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
int UnitAI::combatLogicMageRange(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3])
{
    int line = 0;
    
    int strength[3] = {0, 0, 0};
    for(int i = 0; i < 3; i++)
        strength[i] = ((_friendlies[i] > 0) ? 1 : 0) - ((_enemies[i] > 0) ? 1 : 0);
    
    if(strength[1] == -1)
        line = 2;
    else
    {
        if(strength[0] == -1 && strength[2] == -1)
        {
            if(_friendlies[1] == 0)
                line = 2;
            else
            {
                if(_friendlies[0] == 0 && _friendlies[2] == 0)
                {
                    if(_unitAI->getLine() == 1)
                        line = 1;
                    else if(_unitAI->getLine() == 3)
                        line = 3;
                    else
                    {
                        if(_enemies[0] >= _enemies[2])
                            line = 1;
                        else
                            line = 3;
                    }
                }
                else if(_friendlies[0] == 0)
                    line = 1;
                else if(_friendlies[2] == 0)
                    line = 3;
            }
        }
        else if(strength[0] == -1)
            line = 1;
        else if(strength[2] == -1)
            line = 3;
        else
        {
/** additional logic (find vacant space in priority)
            // for consider themselves
            int f[3];
            for(int i = 0; i < 3; i++)
                f[i] = _friendlies[i];
            f[_unitAI->getTeam() - 1]++;
            //
            
            if( !(f[0] == f[1] == f[2]) )
            {
                int cmp = f[1];
                line = 2;
                
                if(f[0] < cmp)
                {
                    cmp = f[0];
                    line = 1;
                }
                
                if(f[2] < cmp)
                {
                    cmp = f[2];
                    line = 3;
                }
            }
*/
        }
    }
/*
    CCLOG( "COMBAT ORDERING:\n mob: %s\n line 3:  %d - %d = %d\n line 2:  %d - %d = %d\n line 1:  %d - %d = %d\n changing line: %d\n",
          _unitAI->getSkin().c_str(),
          _friendlies[2], _enemies[2], strength[2],
          _friendlies[1], _enemies[1], strength[1],
          _friendlies[0], _enemies[0], strength[0],
          line);
*/
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
void UnitAI::combatChangingLine(AnimatedUnitModel* _unitAI, float _dt)
{
    float   battleFieldLineHeight = Settings::sharedInstance()->getBattleFieldRect().size.height / 3;
    CCPoint nowAiPos = _unitAI->getPosition();
    
    int changingLine = _unitAI->getChangingLine();
    if(changingLine != 0)
    {
        int     necessaryY = battleFieldLineHeight * (changingLine - 1);
        float   changingStep = _unitAI->getMovingSpeed() * _dt * MOB_CHANGING_LINE_SPEED;
        
        if( nowAiPos.y > necessaryY + changingStep ||
           nowAiPos.y < necessaryY - changingStep )
        {
            _unitAI->setMoveTarget( ccp(_unitAI->getMoveTarget().x, necessaryY) );
            
            if(nowAiPos.y > necessaryY)
                _unitAI->setPosition( ccp(nowAiPos.x, nowAiPos.y - changingStep) );
            else
                _unitAI->setPosition( ccp(nowAiPos.x, nowAiPos.y + changingStep) );
            
            _unitAI->setState("move1");
        }
        else
        {
            _unitAI->setLine(changingLine);
            _unitAI->setChangingLine(0);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------