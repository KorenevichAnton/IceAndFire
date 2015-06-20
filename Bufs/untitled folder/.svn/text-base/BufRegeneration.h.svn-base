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
    virtual void    update(AnimatedUnitModel* _unit, const std::vector<AnimatedUnitModel*>& _allUnits, float dt);
    virtual void    disactivate();

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
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufRegeneration__) */
