#ifndef __BattleMagic__ZombieDarkKnightMobAI__
#define __BattleMagic__ZombieDarkKnightMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ZombieDarkKnightMobAI: public UnitAI
{
public:
    static ZombieDarkKnightMobAI*   createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                            damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~ZombieDarkKnightMobAI();

private:
    ZombieDarkKnightMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ZombieDarkKnightMobAI__) */
