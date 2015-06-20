#ifndef __BattleMagic__GarpunBulletAI__
#define __BattleMagic__GarpunBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class GarpunBulletAI: public UnitAI
{
public:
    static GarpunBulletAI*      createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~GarpunBulletAI();

private:
    GarpunBulletAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
    float   splashRange;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__GarpunBulletAI__) */
