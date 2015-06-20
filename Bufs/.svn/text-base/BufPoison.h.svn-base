#ifndef __BattleMagic__BufPoison__
#define __BattleMagic__BufPoison__

#include "Buf.h"

class AnimatedUnitModel;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
class BufPoison: public Buf
{
public:
    static BufPoison*   createTS(BattleModel* _battleModel, int _bufID);

    virtual bool        activate(AnimatedUnitModel* _unit);
    virtual void        update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt);
    virtual void        disactivate();

    float               getDamage() const { return damage; }
    void                setDamage(float _damage) { damage = _damage; }

    float               getCooldown() const { return cooldown; }
    void                setCooldown(float _cooldown) { cooldown = _cooldown; }

    BufPoison*          copy() const;

    virtual ~BufPoison();

protected:
    BufPoison();
    BufPoison(const BufPoison& _other);

    void                initWithXML();

    std::string         skin;

    float               damage;

    float               cooldown;
    float               remainedCoolDown;
    float               passedTime;

    AnimatedUnitModel*  model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufPoison__) */
