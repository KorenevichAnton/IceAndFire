#ifndef __BattleMagic__ZombieMobAI__
#define __BattleMagic__ZombieMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ZombieMobAI: public UnitAI
{
public:
    static ZombieMobAI*      createTS(BattleModel* _battleModel);
    void                     update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                     damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~ZombieMobAI();

private:
    ZombieMobAI();
    
    float       waitAnimation;
    
    void                     init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ZombieMobAI__) */
