#ifndef __BattleMagic__BulletFactory__
#define __BattleMagic__BulletFactory__

#include <stdio.h>
#include "AnimatedUnitModel.h"

//---------------------------------------------------------------------------------------------------------------------------------
class BulletFactory
{
public:
    static AnimatedUnitModel*   createTS(int _bulletID, BattleModel* _battleModel, int _unitID = 0);
    
private:
    static AnimatedUnitModel*   initWithXML(BattleModel* _battleModel, const std::string& _xmlFileName, int _unitID = 0);
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BulletFactory__) */
