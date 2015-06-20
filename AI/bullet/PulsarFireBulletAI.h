#ifndef __BattleMagic__PulsarFireBulletAI__
#define __BattleMagic__PulsarFireBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PulsarFireBulletAI: public UnitAI
{
public:
    static PulsarFireBulletAI*      createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~PulsarFireBulletAI();

private:
    PulsarFireBulletAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__PulsarFireBulletAI__) */
