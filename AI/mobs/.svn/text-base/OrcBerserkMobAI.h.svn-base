#ifndef __BattleMagic__OrcBerserkMobAI__
#define __BattleMagic__OrcBerserkMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class OrcBerserkMobAI: public UnitAI
{
public:
    static OrcBerserkMobAI*     createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~OrcBerserkMobAI();

private:
    OrcBerserkMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    int     raiseDamage;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__OrcBerserkMobAI__) */
