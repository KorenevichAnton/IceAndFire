#ifndef __BattleMagic__WaterArmorSpellAI__
#define __BattleMagic__WaterArmorSpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class WaterArmorSpellAI: public UnitAI
{
public:
    static WaterArmorSpellAI*   createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~WaterArmorSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    WaterArmorSpellAI();
    
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
#endif /* defined(__BattleMagic__WaterArmorSpellAI__) */
