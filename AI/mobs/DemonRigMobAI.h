#ifndef __BattleMagic__DemonRigMobAI__
#define __BattleMagic__DemonRigMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class DemonRigMobAI: public UnitAI
{
public:
    static DemonRigMobAI*       createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~DemonRigMobAI();

private:
    DemonRigMobAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    float       waitAnimation;
    bool        initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__DemonRigMobAI__) */
