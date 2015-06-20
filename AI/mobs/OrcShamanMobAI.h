#ifndef __BattleMagic__OrcShamanMobAI__
#define __BattleMagic__OrcShamanMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class OrcShamanMobAI: public UnitAI
{
public:
    static OrcShamanMobAI*      createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    AnimatedUnitModel*          getFriendly(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, int _priority);

    virtual ~OrcShamanMobAI();

private:
    OrcShamanMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__OrcShamanMobAI__) */
