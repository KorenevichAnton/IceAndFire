#ifndef __BattleMagic__PulsarLightBulletAI__
#define __BattleMagic__PulsarLightBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PulsarLightBulletAI: public UnitAI
{
public:
    static PulsarLightBulletAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~PulsarLightBulletAI();

private:
    PulsarLightBulletAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__PulsarLightBulletAI__) */
