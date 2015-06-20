#ifndef __BattleMagic__KoboldShamanMobAI__
#define __BattleMagic__KoboldShamanMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class KoboldShamanMobAI: public UnitAI
{
public:
    static KoboldShamanMobAI*   createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~KoboldShamanMobAI();

private:
    KoboldShamanMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__KoboldShamanMobAI__) */
