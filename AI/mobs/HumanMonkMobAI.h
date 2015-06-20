#ifndef __BattleMagic__HumanMonkMobAI__
#define __BattleMagic__HumanMonkMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class HumanMonkMobAI: public UnitAI
{
public:
    static HumanMonkMobAI*      createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*          getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);

    virtual ~HumanMonkMobAI();

private:
    HumanMonkMobAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HumanMonkMobAI__) */
