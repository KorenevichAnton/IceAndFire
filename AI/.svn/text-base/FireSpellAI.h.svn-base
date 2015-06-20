//
//  FireSpellAI.h
//  BattleMagic
//
//  Created by Andrey Ryabushkin on 23.02.13.
//
//

#ifndef __BattleMagic__FireSpellAI__
#define __BattleMagic__FireSpellAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class FireSpellAI:public UnitAI
{
private:
    AnimatedUnitModel* _manaSource;
    cocos2d::CCDictionary* _damagedUnitsList;
    std::string         _spellSchoolName;
    
    FireSpellAI();
    
public:
    virtual ~FireSpellAI();
    void update(AnimatedUnitModel* unitAI,const cocos2d::CCArray* Allunits,float dt);
    
    // only for one game mechanics
    void setManaSource(AnimatedUnitModel* manaSource, const std::string& school)
    {
        _manaSource = manaSource;
        _spellSchoolName = school;
    }
    static FireSpellAI* createTS(BattleModel* commands);
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FireSpellAI__) */
