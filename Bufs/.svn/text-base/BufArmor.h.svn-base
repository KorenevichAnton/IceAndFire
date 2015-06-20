#ifndef __BattleMagic__BufArmor__
#define __BattleMagic__BufArmor__

#include "Buf.h"
#include <vector>

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BufArmor: public Buf
{
public:
    static BufArmor*        createTS(BattleModel* _battleModel, int _bufID);

    virtual bool            activate(AnimatedUnitModel* _unit);
    virtual void            update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt);
    virtual void            disactivate();

    BufArmor*               copy() const;

    virtual ~BufArmor();

protected:
    BufArmor();
    BufArmor(const BufArmor& _other);

    void                        initWithXML();

    std::string                 skin;
    float                       power;

    float                       passedTime;
    float                       modelShowTime;

    AnimatedUnitModel*          bufedUnit;
    AnimatedUnitModel*          model;
    
private:
    float                       difference;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufArmor__) */
