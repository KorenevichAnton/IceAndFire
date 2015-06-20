#ifndef __BattleMagic__SceletonWarriorMobAI__
#define __BattleMagic__SceletonWarriorMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class SceletonWarriorMobAI: public UnitAI
{
public:
    static SceletonWarriorMobAI*    createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);


    virtual ~SceletonWarriorMobAI();

private:
    SceletonWarriorMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    float   acceleration;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__SceletonWarriorMobAI__) */
