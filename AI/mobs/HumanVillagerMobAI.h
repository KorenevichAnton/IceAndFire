#ifndef __BattleMagic__HumanVillagerMobAI__
#define __BattleMagic__HumanVillagerMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class HumanVillagerMobAI: public UnitAI
{
public:
    static HumanVillagerMobAI*      createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);


    virtual ~HumanVillagerMobAI();

private:
    HumanVillagerMobAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HumanVillagerMobAI__) */
