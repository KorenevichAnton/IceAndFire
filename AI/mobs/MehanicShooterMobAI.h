#ifndef __BattleMagic__MehanicShooterMobAI__
#define __BattleMagic__MehanicShooterMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class MehanicShooterMobAI: public UnitAI
{
public:
    static MehanicShooterMobAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~MehanicShooterMobAI();

private:
    MehanicShooterMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__MehanicShooterMobAI__) */
