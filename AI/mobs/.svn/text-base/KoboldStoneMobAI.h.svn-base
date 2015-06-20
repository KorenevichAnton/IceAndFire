#ifndef __BattleMagic__KoboldStoneMobAI__
#define __BattleMagic__KoboldStoneMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class KoboldStoneMobAI: public UnitAI
{
public:
    static KoboldStoneMobAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~KoboldStoneMobAI();

private:
    KoboldStoneMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__KoboldStoneMobAI__) */
