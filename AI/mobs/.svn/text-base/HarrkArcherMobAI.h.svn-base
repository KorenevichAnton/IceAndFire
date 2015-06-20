#ifndef __BattleMagic__HarrkArcherMobAI__
#define __BattleMagic__HarrkArcherMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class HarrkArcherMobAI: public UnitAI
{
public:
    static HarrkArcherMobAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~HarrkArcherMobAI();

private:
    HarrkArcherMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HarrkArcherMobAI__) */
