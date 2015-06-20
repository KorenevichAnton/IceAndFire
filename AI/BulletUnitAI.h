//
//  BulletUnitAI.h
//  BattleMagic
//
//  Created by Andrey Ryabushkin on 15.03.13.
//
//

#ifndef __BattleMagic__BulletUnitAI__
#define __BattleMagic__BulletUnitAI__

#include "UnitAI.h"

//---------------------------------------------------------------------------------------------------------------------------------
class BulletUnitAI:public UnitAI
{
private:
    cocos2d::CCDictionary* _damagedUnitsList;
    
    std::set<int>    _exeptionList;
    
    BulletUnitAI();
    
public:
    virtual ~BulletUnitAI();
    void update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits,float dt);
    static BulletUnitAI* createTS(BattleModel* commands);
    
    void addUntouchableUnit(int id){ _exeptionList.insert(id); }
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BulletUnitAI__) */
