#ifndef __BattleMagic__FrostlingTrapperMobAI__
#define __BattleMagic__FrostlingTrapperMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class FrostlingTrapperMobAI: public UnitAI
{
public:
    static FrostlingTrapperMobAI*   createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~FrostlingTrapperMobAI();

private:
    FrostlingTrapperMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    float   panicTime;
    float   panicTimeRemained;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FrostlingTrapperMobAI__) */
