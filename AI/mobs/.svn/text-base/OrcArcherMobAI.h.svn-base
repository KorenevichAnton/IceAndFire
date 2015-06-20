#ifndef __BattleMagic__OrcArcherMobAI__
#define __BattleMagic__OrcArcherMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class OrcArcherMobAI: public UnitAI
{
public:
    static OrcArcherMobAI*      createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~OrcArcherMobAI();

private:
    OrcArcherMobAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
    int     reduce;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__OrcArcherMobAI__) */
