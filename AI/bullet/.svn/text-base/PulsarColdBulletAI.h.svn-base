#ifndef __BattleMagic__PulsarColdBulletAI__
#define __BattleMagic__PulsarColdBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PulsarColdBulletAI: public UnitAI
{
public:
    static PulsarColdBulletAI*      createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~PulsarColdBulletAI();

private:
    PulsarColdBulletAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__PulsarColdBulletAI__) */
