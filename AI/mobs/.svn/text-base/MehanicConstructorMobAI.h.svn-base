#ifndef __BattleMagic__MehanicConstructorMobAI__
#define __BattleMagic__MehanicConstructorMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class MehanicConstructorMobAI: public UnitAI
{
public:
    static MehanicConstructorMobAI*     createTS(BattleModel* _battleModel);
    void                                update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*                  getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);

    virtual ~MehanicConstructorMobAI();

private:
    MehanicConstructorMobAI();
    
    float       waitAnimation;
    
    void                                init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__MehanicConstructorMobAI__) */
