#ifndef __BattleMagic__OrcDubolomMobAI__
#define __BattleMagic__OrcDubolomMobAI__

#include "UnitAI.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class OrcDubolomMobAI: public UnitAI
{
public:
    static OrcDubolomMobAI*     createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    bool                        canMove(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    virtual ~OrcDubolomMobAI();

private:
    OrcDubolomMobAI();
    
    float       waitAnimation;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool    initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__OrcDubolomMobAI__) */
