#ifndef __BattleMagic__AirSpeedSpellAI__
#define __BattleMagic__AirSpeedSpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class AirSpeedSpellAI: public UnitAI
{
public:
    static AirSpeedSpellAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~AirSpeedSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    
    //For init casting area;
    cocos2d::CCPoint position;
    
    cocos2d::CCPoint firstCPPosition;
    cocos2d::CCPoint secondCPPosition;
    
    void                        initCastingArea();
    //
    
    AirSpeedSpellAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isBufed(AnimatedUnitModel* _unit);

    bool    initialize;
    std::vector<AnimatedUnitModel*>     bufedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__AirSpeedSpellAI__) */
