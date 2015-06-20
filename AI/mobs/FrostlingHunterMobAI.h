#ifndef __BattleMagic__FrostlingHunterMobAI__
#define __BattleMagic__FrostlingHunterMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class FrostlingHunterMobAI: public UnitAI
{
public:
    static FrostlingHunterMobAI*    createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~FrostlingHunterMobAI();

private:
    FrostlingHunterMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FrostlingHunterMobAI__) */
