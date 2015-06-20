#ifndef __BattleMagic__ZvergMysticMobAI__
#define __BattleMagic__ZvergMysticMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class ZvergMysticMobAI: public UnitAI
{
public:
    static ZvergMysticMobAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*          getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);

    virtual ~ZvergMysticMobAI();

private:
    ZvergMysticMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ZvergMysticMobAI__) */
