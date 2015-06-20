#ifndef __BattleMagic__BufFire__
#define __BattleMagic__BufFire__

#include "Buf.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BufFire: public Buf
{
public:
    static BufFire* createTS(BattleModel* _battleModel, int _bufID);

    virtual bool    activate(AnimatedUnitModel* _unit);
    virtual void    update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt);
    virtual void    disactivate();

    BufFire*        copy() const;

    virtual ~BufFire();

protected:
    BufFire();
    BufFire(const BufFire& _other);

    void                initWithXML();

    std::string         skin;
    float               damage;
    float               range;

    float               cooldown;
    float               remainedCoolDown;
    float               passedTime;

    AnimatedUnitModel*  model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufFire__) */
