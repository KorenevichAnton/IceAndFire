#ifndef __BattleMagic__FireWallBulletAI__
#define __BattleMagic__FireWallBulletAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class FireWallBulletAI: public UnitAI
{
public:
    static FireWallBulletAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits, float _dt);

    virtual ~FireWallBulletAI();

private:
    FireWallBulletAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray *allUnits);
    
    bool    initialize;
    float   remainedCoolDown;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FireWallBulletAI__) */
