#ifndef __BattleMagic__FireRainSpellAI__
#define __BattleMagic__FireRainSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class FireRainSpellAI: public UnitAI
{
public:
    static FireRainSpellAI*     createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~FireRainSpellAI();
    
protected:
    ArtificialIntelligenceType  typeUnitAI;

private:
    FireRainSpellAI();
    
    //For init casting area;
    cocos2d::CCPoint position;
    
    cocos2d::CCPoint firstCPPosition;
    cocos2d::CCPoint secondCPPosition;
    
    void                        initCastingArea();
    //
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isAttacked(AnimatedUnitModel* _unit);

    bool                    initialize;
    float                   remainedCoolDown;
    
    std::string             skin1;

    std::vector<AnimatedUnitModel*>     attackedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FireRainSpellAI__) */
