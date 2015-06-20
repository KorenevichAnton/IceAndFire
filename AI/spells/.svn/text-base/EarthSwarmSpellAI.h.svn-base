#ifndef __BattleMagic__EarthSwarmSpellAI__
#define __BattleMagic__EarthSwarmSpellAI__

#include "UnitAI.h"

class AnimatedUnitModel;

struct attackedUnit
{
    AnimatedUnitModel*  unit;
    float               remainedCoolDown;
};

//---------------------------------------------------------------------------------------------------------------------------------
class EarthSwarmSpellAI: public UnitAI
{
public:
    static EarthSwarmSpellAI*   createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~EarthSwarmSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    EarthSwarmSpellAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    attackedUnit*               checkAttacked(AnimatedUnitModel* _unit);

    bool                        initialize;
    std::vector<attackedUnit*>  attackedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__EarthSwarmSpellAI__) */
