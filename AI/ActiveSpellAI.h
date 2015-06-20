//
//  ActiveSpellAI.h
//  BattleMagic
//
//  Created by madmin on 12/25/13.
//
//

#ifndef __BattleMagic__ActiveSpellAI__
#define __BattleMagic__ActiveSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class ActiveSpellAI:public UnitAI
{
private:
    cocos2d::CCDictionary* _damagedUnitsList;
    
    std::set<int>    _exeptionList;
    
    ActiveSpellAI();
    
public:
    virtual ~ActiveSpellAI();
    void update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits,float dt);
    static ActiveSpellAI* createTS(BattleModel* commands);
    
    void addUntouchableUnit(int id){ _exeptionList.insert(id); }
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__BattleMagic__ActiveSpellAI__) */
