#ifndef __BattleMagic__MehanicGuardMobAI__
#define __BattleMagic__MehanicGuardMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class MehanicGuardMobAI: public UnitAI
{
public:
    static MehanicGuardMobAI*       createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~MehanicGuardMobAI();

private:
    MehanicGuardMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__MehanicGuardMobAI__) */
