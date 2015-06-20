#ifndef __BattleMagic__Spear2BulletAI__
#define __BattleMagic__Spear2BulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class Spear2BulletAI: public UnitAI
{
public:
    static Spear2BulletAI*   createTS(BattleModel* _battleModel);
    void                     update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~Spear2BulletAI();

private:
    Spear2BulletAI();

    void                    init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__Spear2BulletAI__) */
