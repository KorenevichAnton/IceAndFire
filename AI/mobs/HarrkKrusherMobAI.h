#ifndef __BattleMagic__HarrkKrusherMobAI__
#define __BattleMagic__HarrkKrusherMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class HarrkKrusherMobAI: public UnitAI
{
public:
    static HarrkKrusherMobAI*   createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~HarrkKrusherMobAI();

private:
    HarrkKrusherMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
    int         proportion;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HarrkKrusherMobAI__) */
