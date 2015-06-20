#ifndef __BattleMagic__FrostlingShamanMobAI__
#define __BattleMagic__FrostlingShamanMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class FrostlingShamanMobAI: public UnitAI
{
public:
    static FrostlingShamanMobAI*    createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~FrostlingShamanMobAI();

private:
    FrostlingShamanMobAI();

    float       waitAnimation;
    
    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FrostlingShamanMobAI__) */
