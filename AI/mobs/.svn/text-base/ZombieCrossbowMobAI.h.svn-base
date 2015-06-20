#ifndef __BattleMagic__ZombieCrossbowMobAI__
#define __BattleMagic__ZombieCrossbowMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ZombieCrossbowMobAI: public UnitAI
{
public:
    static ZombieCrossbowMobAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                            damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~ZombieCrossbowMobAI();

private:
    ZombieCrossbowMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ZombieCrossbowMobAI__) */
