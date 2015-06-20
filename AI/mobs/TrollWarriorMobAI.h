#ifndef __BattleMagic__TrollWarriorMobAI__
#define __BattleMagic__TrollWarriorMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class TrollWarriorMobAI: public UnitAI
{
public:
    static TrollWarriorMobAI*   createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~TrollWarriorMobAI();

private:
    TrollWarriorMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
    int         resist;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__TrollWarriorMobAI__) */
