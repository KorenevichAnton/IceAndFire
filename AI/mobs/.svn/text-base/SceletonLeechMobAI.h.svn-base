#ifndef __BattleMagic__SceletonLeechMobAI__
#define __BattleMagic__SceletonLeechMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class SceletonLeechMobAI: public UnitAI
{
public:
    static SceletonLeechMobAI*  createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*          getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);

    virtual ~SceletonLeechMobAI();

private:
    SceletonLeechMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__SceletonLeechMobAI__) */
