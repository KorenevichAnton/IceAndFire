#ifndef __BattleMagic__StoneBulletAI__
#define __BattleMagic__StoneBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class StoneBulletAI: public UnitAI
{
public:
    static StoneBulletAI*    createTS(BattleModel* _battleModel);
    void                     update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~StoneBulletAI();

private:
    StoneBulletAI();

    void                     init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__StoneBulletAI__) */
