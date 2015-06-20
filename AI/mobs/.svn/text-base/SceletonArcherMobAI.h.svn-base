#ifndef __BattleMagic__SceletonArcherMobAI__
#define __BattleMagic__SceletonArcherMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class SceletonArcherMobAI: public UnitAI
{
public:
    static SceletonArcherMobAI* createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~SceletonArcherMobAI();

private:
    SceletonArcherMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__SceletonArcherMobAI__) */
