//
//  PassiveSpellAI.h
//  BattleMagic
//
//  Created by madmin on 12/24/13.
//
//

#ifndef __BattleMagic__PassiveSpellAI__
#define __BattleMagic__PassiveSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PassiveSpellAI:public UnitAI
{
private:
    cocos2d::CCDictionary* _damagedUnitsList;
    
    std::set<int>    _exeptionList;
    
    PassiveSpellAI();
    
public:
    virtual ~PassiveSpellAI();
    void update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits,float dt);
    static PassiveSpellAI* createTS(BattleModel* commands);
    
    void addUntouchableUnit(int id){ _exeptionList.insert(id); }
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__BattleMagic__PassiveSpellAI__) */
