#ifndef __BattleMagic__BufResurrection__
#define __BattleMagic__BufResurrection__

#include "Buf.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufResurrection: public Buf
{
public:
    static BufResurrection* createTS(BattleModel* _battleModel, int _bufID);

    virtual bool    activate(AnimatedUnitModel* _activator);
    virtual void    update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float dt);
    virtual void    disactivate();

    float           getPower() const { return power; }
    void            setPower(float _power) { power = _power; }

    float           getThreshold() const { return threshold; }
    void            setThreshold(float _threshold) { threshold = _threshold; }

    float           getCooldown() const { return cooldown; }
    void            setCooldown(float _cooldown) { cooldown = _cooldown; }

    BufResurrection*        copy() const;

    virtual ~BufResurrection();

protected:
    void            initWithXML();

    BufResurrection();
    BufResurrection(const BufResurrection& _other);

    std::string         skin;

    float               power;
    float               threshold;

    float               cooldown;
    float               remainedCoolDown;
    float               passedTime;

    AnimatedUnitModel*  bufedUnit;
    AnimatedUnitModel*  model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufResurrection__) */
