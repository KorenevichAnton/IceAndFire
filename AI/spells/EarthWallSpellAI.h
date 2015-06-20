#ifndef __BattleMagic__EarthWallSpellAI__
#define __BattleMagic__EarthWallSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class EarthWallSpellAI: public UnitAI
{
public:
    static EarthWallSpellAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    void setPosition(cocos2d::CCPoint _position);
    cocos2d::CCPoint getPosition();
    
    virtual ~EarthWallSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;
    
private:
    EarthWallSpellAI();
    
    cocos2d::CCPoint position;
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    
    bool            initialize;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__EarthWallSpellAI__) */
