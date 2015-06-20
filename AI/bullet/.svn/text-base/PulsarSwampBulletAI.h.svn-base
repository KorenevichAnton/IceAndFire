#ifndef __BattleMagic__PulsarSwampBulletAI__
#define __BattleMagic__PulsarSwampBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PulsarSwampBulletAI: public UnitAI
{
public:
    static PulsarSwampBulletAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~PulsarSwampBulletAI();

private:
    PulsarSwampBulletAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__PulsarSwampBulletAI__) */
