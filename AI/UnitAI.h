#ifndef __BattleMagic__UnitAI__
#define __BattleMagic__UnitAI__

#include "cocos2d.h"
#include "EnumsUnitAI.h"
#include "Settings.h"
#include "AppUtils.h"

#include <vector>

class AnimatedUnitModel;
class BattleModel;
class UnitAttack;

// Interface for Artificial Intelligence (AI)
//---------------------------------------------------------------------------------------------------------------------------------
class UnitAI:public cocos2d::CCObject
{
public:
    ArtificialIntelligenceType  getTypeUnitAI() const { return typeUnitAI; }
    
    virtual void                update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allunits, float dt) = 0;
    virtual void                damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg){};
    
    //capture checkpoint
    bool                        tryCaptureCheckpoint(AnimatedUnitModel* _unit);
    bool                        checkCheckpoint(AnimatedUnitModel* _unit);
    //
    
    UnitAI();
    virtual ~UnitAI();
    
protected:
    //action ordering
    virtual bool                canAttack(AnimatedUnitModel* attacker, AnimatedUnitModel* target, int& priority);
    virtual float               canAction(AnimatedUnitModel* attacker, const cocos2d::CCArray* allunits, int& priority);
    virtual bool                canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allunits);
    
    virtual AnimatedUnitModel*  getEnemy(AnimatedUnitModel* unitAI,const cocos2d::CCArray* allUnits, int priority);
    virtual AnimatedUnitModel*  getFriendly(AnimatedUnitModel* unitAI,const cocos2d::CCArray* allUnits, int priority);
    
    bool                        isCanFinishUndead(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority);
    
    bool                        isBodyCircleInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority);
    bool                        isBodyPolygonInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority);
    
    virtual void                useActionCooldown(AnimatedUnitModel* _unitAI, int _priority);
    void                        updateActionsCooldown(AnimatedUnitModel* _unitAI, float _dt);
    //
    
    //combat ordering
    void                        combatOrdering(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float dt);
    
    virtual int                 combatLogicDexMelee(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3]);
    virtual int                 combatLogicPhysRange(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3]);
    virtual int                 combatLogicStrMelee(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3]);
    virtual int                 combatLogicMageRange(const AnimatedUnitModel* _unitAI, const int _friendlies[3], const int _enemies[3]);
    
    virtual void                combatChangingLine(AnimatedUnitModel* _unitAI, float _dt);
    //

protected:
    ArtificialIntelligenceType  typeUnitAI;
    BattleModel*                pBattleModel;
    Settings*                   _settings;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__UnitAI__) */
