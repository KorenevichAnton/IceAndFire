#ifndef __BattleMagic__DwarfEngineerMobAI__
#define __BattleMagic__DwarfEngineerMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class DwarfEngineerMobAI: public UnitAI
{
public:
    static DwarfEngineerMobAI*  createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    
    virtual ~DwarfEngineerMobAI();

private:
    DwarfEngineerMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
    float   summonOffset;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__DwarfEngineerMobAI__) */
