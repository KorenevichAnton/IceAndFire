#ifndef __BattleMagic__TrapBulletAI__
#define __BattleMagic__TrapBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class TrapBulletAI: public UnitAI
{
public:
    static TrapBulletAI*    createTS(BattleModel* _battleModel);
    void                    update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~TrapBulletAI();

private:
    TrapBulletAI();

    void                    init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__TrapBulletAI__) */
