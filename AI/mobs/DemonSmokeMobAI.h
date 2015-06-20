#ifndef __BattleMagic__DemonSmokeMobAI__
#define __BattleMagic__DemonSmokeMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class DemonSmokeMobAI: public UnitAI
{
public:
    static DemonSmokeMobAI*         createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*              getEnemy(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);
    bool                            canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    void                            damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~DemonSmokeMobAI();

private:
    DemonSmokeMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    float   waitAnimation;
    
    float   specialAnimationDuration;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__DemonSmokeMobAI__) */
