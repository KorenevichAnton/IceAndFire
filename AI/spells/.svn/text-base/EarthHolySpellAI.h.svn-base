#ifndef __BattleMagic__EarthHolySpellAI__
#define __BattleMagic__EarthHolySpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class EarthHolySpellAI: public UnitAI
{
public:
    static EarthHolySpellAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~EarthHolySpellAI();

protected:
    ArtificialIntelligenceType      typeUnitAI;
    
private:
    EarthHolySpellAI();
    
    //For init casting area;
    cocos2d::CCPoint position;
    
    cocos2d::CCPoint firstCPPosition;
    cocos2d::CCPoint secondCPPosition;
    
    void                        initCastingArea();
    //
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isBufed(AnimatedUnitModel* _unit);

    bool        initialize;
    float       threshold;

    std::vector<AnimatedUnitModel*>     bufedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__EarthHolySpellAI__) */
