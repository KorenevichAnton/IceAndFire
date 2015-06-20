#ifndef __BattleMagic__PulsarEarthBulletAI__
#define __BattleMagic__PulsarEarthBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PulsarEarthBulletAI: public UnitAI
{
public:
    static PulsarEarthBulletAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~PulsarEarthBulletAI();

private:
    PulsarEarthBulletAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__PulsarEarthBulletAI__) */
