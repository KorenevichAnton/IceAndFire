#ifndef __BattleMagic__FrostlingWarriorMobAI__
#define __BattleMagic__FrostlingWarriorMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class FrostlingWarriorMobAI: public UnitAI
{
public:
    static FrostlingWarriorMobAI*   createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                            damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~FrostlingWarriorMobAI();

private:
    FrostlingWarriorMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FrostlingWarriorMobAI__) */
