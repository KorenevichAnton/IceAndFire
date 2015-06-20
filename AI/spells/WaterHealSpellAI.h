#ifndef __BattleMagic__WaterHealSpellAI__
#define __BattleMagic__WaterHealSpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class WaterHealSpellAI: public UnitAI
{
public:
    static WaterHealSpellAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~WaterHealSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    WaterHealSpellAI();
    
    //For init casting area;
    cocos2d::CCPoint position;
    
    cocos2d::CCPoint firstCPPosition;
    cocos2d::CCPoint secondCPPosition;
    
    void                        initCastingArea();
    //
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isBufed(AnimatedUnitModel* _unit);
    
    bool            initialize;

    std::string     skin1;
    
    std::vector<AnimatedUnitModel*>     bufedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__WaterHealSpellAI__) */
