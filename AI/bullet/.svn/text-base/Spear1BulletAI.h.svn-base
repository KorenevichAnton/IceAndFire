#ifndef __BattleMagic__Spear1BulletAI__
#define __BattleMagic__Spear1BulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class Spear1BulletAI: public UnitAI
{
public:
    static Spear1BulletAI*  createTS(BattleModel* _battleModel);
    void                    update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~Spear1BulletAI();

private:
    Spear1BulletAI();

    void                    init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__Spear1BulletAI__) */
