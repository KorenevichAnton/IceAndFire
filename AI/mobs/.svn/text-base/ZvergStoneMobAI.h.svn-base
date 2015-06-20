#ifndef __BattleMagic__ZvergStoneMobAI__
#define __BattleMagic__ZvergStoneMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ZvergStoneMobAI: public UnitAI
{
public:
    static ZvergStoneMobAI*     createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~ZvergStoneMobAI();

private:
    ZvergStoneMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ZvergStoneMobAI__) */
