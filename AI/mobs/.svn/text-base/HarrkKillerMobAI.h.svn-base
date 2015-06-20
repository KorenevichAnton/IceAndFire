#ifndef __BattleMagic__HarrkKillerMobAI__
#define __BattleMagic__HarrkKillerMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class HarrkKillerMobAI: public UnitAI
{
public:
    static HarrkKillerMobAI*        createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~HarrkKillerMobAI();

private:
    HarrkKillerMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HarrkKillerMobAI__) */
