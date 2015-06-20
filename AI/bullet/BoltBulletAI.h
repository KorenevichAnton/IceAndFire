#ifndef __BattleMagic__BoltBulletAI__
#define __BattleMagic__BoltBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class BoltBulletAI: public UnitAI
{
public:
    static BoltBulletAI*    createTS(BattleModel* _battleModel);
    void                     update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~BoltBulletAI();

private:
    BoltBulletAI();

    void                     init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
    float   redoubleRange;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BoltBulletAI__) */
