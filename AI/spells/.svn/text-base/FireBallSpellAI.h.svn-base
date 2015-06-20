#ifndef __BattleMagic__FireBallSpellAI__
#define __BattleMagic__FireBallSpellAI__

#include "UnitAI.h"


//---------------------------------------------------------------------------------------------------------------------------------
class FireBallSpellAI:public UnitAI
{
private:
    cocos2d::CCDictionary* _damagedUnitsList;
    
    std::set<int>    _exeptionList;
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* allUnits);
    bool             initialize;
    
    FireBallSpellAI();
    
protected:
    ArtificialIntelligenceType      typeUnitAI;
   
public:
    virtual ~FireBallSpellAI();
    void update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits,float dt);
    static FireBallSpellAI* createTS(BattleModel* commands);
    
    void addUntouchableUnit(int id){ _exeptionList.insert(id); }
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FireBallSpellAI__) */
