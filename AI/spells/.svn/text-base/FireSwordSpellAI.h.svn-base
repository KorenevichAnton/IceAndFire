#ifndef __BattleMagic__FireSwordSpellAI__
#define __BattleMagic__FireSwordSpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class FireSwordSpellAI: public UnitAI
{
public:
    static FireSwordSpellAI*    createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    void setPosition(cocos2d::CCPoint _position);
    
    virtual ~FireSwordSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    FireSwordSpellAI();

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
#endif /* defined(__BattleMagic__FireSwordSpellAI__) */
