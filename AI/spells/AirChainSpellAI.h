#ifndef __BattleMagic__AirChainSpellAI__
#define __BattleMagic__AirChainSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class AirChainSpellAI: public UnitAI
{
public:
    static AirChainSpellAI*     createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);

    virtual ~AirChainSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;

private:
    AirChainSpellAI();

    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    bool                        isAttacked(AnimatedUnitModel* _unit);

    bool                initialize;
    float               remainedCoolDown;

    int                 remainedCountAttack;
    int                 damageDec;      //decrease damage in percent
    
    std::string         skin1;
    std::string         skin2;
    
    std::vector<AnimatedUnitModel*>     attackedUnits;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__AirChainSpellAI__) */
