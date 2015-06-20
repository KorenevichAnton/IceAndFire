#ifndef __BattleMagic__WaterNeedleSpellAI__
#define __BattleMagic__WaterNeedleSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class WaterNeedleSpellAI: public UnitAI
{
public:
    static WaterNeedleSpellAI*  createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~WaterNeedleSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    WaterNeedleSpellAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool            initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__WaterNeedleSpellAI__) */
