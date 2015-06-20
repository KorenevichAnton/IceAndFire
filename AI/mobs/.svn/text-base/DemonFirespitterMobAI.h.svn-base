#ifndef __BattleMagic__DemonFirespitterMobAI__
#define __BattleMagic__DemonFirespitterMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class DemonFirespitterMobAI: public UnitAI
{
public:
    static DemonFirespitterMobAI*   createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    AnimatedUnitModel*              getEnemy(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);
    bool                            canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    void                            damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~DemonFirespitterMobAI();

private:
    DemonFirespitterMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    float   waitAnimation;
    float   specialAnimationDuration;
    float   waitAnimationRemained;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__DemonFirespitterMobAI__) */
