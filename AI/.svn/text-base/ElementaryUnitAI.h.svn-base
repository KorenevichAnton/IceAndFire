//
//  ElementaryUnitAI.h
//  BattleMagic
//
//  Created by Andrey Ryabushkin on 23.02.13.
//
//

#ifndef __BattleMagic__ElementaryUnitAI__
#define __BattleMagic__ElementaryUnitAI__

#include "Settings.h"
#include "UnitAI.h"
//---------------------------------------------------------------------------------------------------------------------------------
class ElementaryUnitAI:public UnitAI
{
    //ability
    int countPush;
    int remainingCountPush;
    //
protected:
    Settings* _settings;
    
    void update(AnimatedUnitModel* unitAI,const cocos2d::CCArray* Allunits,float dt);
        
    void landingZoneAccess(AnimatedUnitModel* unitAI, const cocos2d::CCArray *allUnits);
    cocos2d::CCPoint pathFind(const AnimatedUnitModel* unitAI,const AnimatedUnitModel* unit);
    
    ElementaryUnitAI();
public:
    virtual ~ElementaryUnitAI();
    
    static ElementaryUnitAI* createTS(BattleModel* _commands);
    
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ElementaryUnitAI__) */
