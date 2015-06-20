#ifndef __BattleMagic__DemonSlaveMasterMobAI__
#define __BattleMagic__DemonSlaveMasterMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class DemonSlaveMasterMobAI: public UnitAI
{
public:
    static DemonSlaveMasterMobAI*   createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*              getEnemy(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);
    AnimatedUnitModel*              getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);
    bool                            canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    void                            damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~DemonSlaveMasterMobAI();

private:
    DemonSlaveMasterMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    float   waitAnimation;
    float   specialAnimationDuration;
    float   fireWallLifeTime;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__DemonSlaveMasterMobAI__) */
