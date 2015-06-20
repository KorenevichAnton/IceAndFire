#ifndef __BattleMagic__KoboldStaratelMobAI__
#define __BattleMagic__KoboldStaratelMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class KoboldStaratelMobAI: public UnitAI
{
public:
    static KoboldStaratelMobAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~KoboldStaratelMobAI();

private:
    KoboldStaratelMobAI();
    
    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__KoboldStaratelMobAI__) */
