#ifndef __BattleMagic__BufRegeneration__
#define __BattleMagic__BufRegeneration__

#include "Buf.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufRegeneration: public Buf
{
public:
    static BufRegeneration* createTS(BattleModel* _battleModel, int _bufID);

    virtual bool    activate(AnimatedUnitModel* _activator);
    virtual void    update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float dt);
    virtual void    disactivate();

    float           getPower() const { return power; }
    void            setPower(float _power) { power = _power; }

    float           getCooldown() const { return cooldown; }
    void            setCooldown(float _cooldown) { cooldown = _cooldown; }

    BufRegeneration*        copy() const;

    virtual ~BufRegeneration();

protected:
    void            initWithXML();

    BufRegeneration();
    BufRegeneration(const BufRegeneration& _other);

    std::string         skin;

    float               power;

    float               cooldown;
    float               remainedCoolDown;
    float               passedTime;

    AnimatedUnitModel*  bufedUnit;
    AnimatedUnitModel*  model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufRegeneration__) */
