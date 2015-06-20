#ifndef __BattleMagic__KoboldWarriorMobAI__
#define __BattleMagic__KoboldWarriorMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class KoboldWarriorMobAI: public UnitAI
{
public:
    static KoboldWarriorMobAI*  createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~KoboldWarriorMobAI();

private:
    KoboldWarriorMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool        initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__KoboldWarriorMobAI__) */
