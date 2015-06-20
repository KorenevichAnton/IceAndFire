#ifndef __BattleMagic__HumanArcherMobAI__
#define __BattleMagic__HumanArcherMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class HumanArcherMobAI: public UnitAI
{
public:
    static HumanArcherMobAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);


    virtual ~HumanArcherMobAI();

private:
    HumanArcherMobAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    float       waitAnimation;
    
    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HumanArcherMobAI__) */
