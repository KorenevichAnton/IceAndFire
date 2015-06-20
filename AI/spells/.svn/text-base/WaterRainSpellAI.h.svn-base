#ifndef __BattleMagic__WaterRainSpellAI__
#define __BattleMagic__WaterRainSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class WaterRainSpellAI: public UnitAI
{
public:
    static WaterRainSpellAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~WaterRainSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    WaterRainSpellAI();
    
    //For init casting area;
    cocos2d::CCPoint position;
    
    cocos2d::CCPoint firstCPPosition;
    cocos2d::CCPoint secondCPPosition;
    
    void                        initCastingArea();
    //
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isAttacked(AnimatedUnitModel* _unit);

    bool            initialize;
    float           remainedCoolDown;

    std::string     skin1;

    std::vector<AnimatedUnitModel*>     attackedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__WaterRainSpellAI__) */
