#ifndef __BattleMagic__ArrowBulletAI__
#define __BattleMagic__ArrowBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class ArrowBulletAI: public UnitAI
{
public:
    static ArrowBulletAI*    createTS(BattleModel* _battleModel);
    void                     update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~ArrowBulletAI();

private:
    ArrowBulletAI();
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ArrowBulletAI__) */
