#ifndef __BattleMagic__BugGreenMobAI__
#define __BattleMagic__BugGreenMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BugGreenMobAI: public UnitAI
{
public:
    static BugGreenMobAI*           createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~BugGreenMobAI();

private:
    BugGreenMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BugGreenMobAI__) */
