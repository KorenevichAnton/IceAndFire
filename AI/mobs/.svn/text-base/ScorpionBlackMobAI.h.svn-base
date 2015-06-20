#ifndef __BattleMagic__ScorpionBlackMobAI__
#define __BattleMagic__ScorpionBlackMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ScorpionBlackMobAI: public UnitAI
{
public:
    static ScorpionBlackMobAI*      createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);


    virtual ~ScorpionBlackMobAI();

private:
    ScorpionBlackMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ScorpionBlackMobAI__) */
