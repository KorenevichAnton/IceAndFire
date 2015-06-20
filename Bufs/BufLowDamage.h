#ifndef __BattleMagic__BufLowDamage__
#define __BattleMagic__BufLowDamage__

#include "Buf.h"
#include <vector>

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BufLowDamage: public Buf
{
public:
    static BufLowDamage*    createTS(BattleModel* _battleModel, int _bufID);

    virtual bool            activate(AnimatedUnitModel* _unit);
    virtual void            update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt);
    virtual void            disactivate();

    BufLowDamage*           copy() const;

    virtual ~BufLowDamage();

protected:
    BufLowDamage();
    BufLowDamage(const BufLowDamage& _other);

    void                    initWithXML();

    std::string                 skin;
    float                       power;

    float                       passedTime;

    AnimatedUnitModel*          bufedUnit;
    AnimatedUnitModel*          model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufLowDamage__) */
