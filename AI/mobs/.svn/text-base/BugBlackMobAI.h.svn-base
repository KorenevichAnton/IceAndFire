#ifndef __BattleMagic__BugBlackMobAI__
#define __BattleMagic__BugBlackMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BugBlackMobAI: public UnitAI
{
public:
    static BugBlackMobAI*           createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~BugBlackMobAI();

private:
    BugBlackMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BugBlackMobAI__) */
