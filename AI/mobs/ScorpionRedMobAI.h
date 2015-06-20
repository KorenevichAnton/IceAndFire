#ifndef __BattleMagic__ScorpionRedMobAI__
#define __BattleMagic__ScorpionRedMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ScorpionRedMobAI: public UnitAI
{
public:
    static ScorpionRedMobAI*        createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~ScorpionRedMobAI();

private:
    ScorpionRedMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    float                           waitAnimation;
    
    bool                            initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ScorpionRedMobAI__) */
