#ifndef __BattleMagic__AirLightningSpellAI__
#define __BattleMagic__AirLightningSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class AirLightningSpellAI: public UnitAI
{
public:
    static AirLightningSpellAI*     createTS(BattleModel* _battleModel);
    void                            update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~AirLightningSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    AirLightningSpellAI();

    void                            init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);

    bool            initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__AirLightningSpellAI__) */
