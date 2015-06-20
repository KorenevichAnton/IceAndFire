#ifndef __BattleMagic__DwarfStoneMobAI__
#define __BattleMagic__DwarfStoneMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class DwarfStoneMobAI: public UnitAI
{
public:
    static DwarfStoneMobAI*         createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void                        damaged(AnimatedUnitModel* _unitAI, AnimatedUnitModel* _attacker, const UnitAttack* _attack, float& _realDmg);

    virtual ~DwarfStoneMobAI();

private:
    DwarfStoneMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
    int         resist;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__DwarfStoneMobAI__) */
