#ifndef __BattleMagic__ZvergCrossbowMobAI__
#define __BattleMagic__ZvergCrossbowMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ZvergCrossbowMobAI: public UnitAI
{
public:
    static ZvergCrossbowMobAI*  createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~ZvergCrossbowMobAI();

private:
    ZvergCrossbowMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ZvergCrossbowMobAI__) */
