#ifndef __BattleMagic__EarthPowerSpellAI__
#define __BattleMagic__EarthPowerSpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class EarthPowerSpellAI: public UnitAI
{
public:
    static EarthPowerSpellAI*   createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~EarthPowerSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    EarthPowerSpellAI();
    
    //For init casting area;
    cocos2d::CCPoint position;
    
    cocos2d::CCPoint firstCPPosition;
    cocos2d::CCPoint secondCPPosition;
    
    void                        initCastingArea();
    //
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isBufed(AnimatedUnitModel* _unit);

    bool    initialize;
    std::vector<AnimatedUnitModel*>     bufedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__EarthPowerSpellAI__) */
