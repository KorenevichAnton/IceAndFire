#ifndef __BattleMagic__BufFireSword__
#define __BattleMagic__BufFireSword__

#include "Buf.h"
#include <vector>

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BufFireSword: public Buf
{
public:
    static BufFireSword*    createTS(BattleModel* _battleModel, int _bufID);

    virtual bool            activate(AnimatedUnitModel* _unit);
    virtual void            update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt);
    virtual void            disactivate();

    BufFireSword*           copy() const;

    virtual ~BufFireSword();

protected:
    BufFireSword();
    BufFireSword(const BufFireSword& _other);

    void                    initWithXML();

    std::string                 skin;
    float                       power;

    float                       passedTime;
    float                       modelShowTime;

    AnimatedUnitModel*          bufedUnit;
    std::vector< std::string >  prevSchools;
    
    AnimatedUnitModel*          model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufFireSword__) */
