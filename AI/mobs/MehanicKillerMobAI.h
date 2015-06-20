#ifndef __BattleMagic__MehanicKillerMobAI__
#define __BattleMagic__MehanicKillerMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class MehanicKillerMobAI: public UnitAI
{
public:
    static MehanicKillerMobAI*      createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~MehanicKillerMobAI();

private:
    MehanicKillerMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__MehanicKillerMobAI__) */
