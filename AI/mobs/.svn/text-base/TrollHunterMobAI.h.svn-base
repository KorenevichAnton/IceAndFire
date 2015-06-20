#ifndef __BattleMagic__TrollHunterMobAI__
#define __BattleMagic__TrollHunterMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class TrollHunterMobAI: public UnitAI
{
public:
    static TrollHunterMobAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~TrollHunterMobAI();

private:
    TrollHunterMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__TrollHunterMobAI__) */
