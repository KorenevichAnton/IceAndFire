#ifndef __BattleMagic__BulletBulletAI__
#define __BattleMagic__BulletBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class BulletBulletAI: public UnitAI
{
public:
    static BulletBulletAI*  createTS(BattleModel* _battleModel);
    void                    update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~BulletBulletAI();

private:
    BulletBulletAI();

    void                    init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BulletBulletAI__) */
