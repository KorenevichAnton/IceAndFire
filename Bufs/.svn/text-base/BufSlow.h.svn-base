#ifndef __BattleMagic__BufSlow__
#define __BattleMagic__BufSlow__

#include "Buf.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufSlow: public Buf
{
public:
    static BufSlow* createTS(BattleModel* _battleModel, int _bufID);

    virtual bool    activate(AnimatedUnitModel* _activator);
    virtual void    update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float dt);
    virtual void    disactivate();

    BufSlow*        copy() const;

    virtual ~BufSlow();

protected:
    void            initWithXML();

    BufSlow();
    BufSlow(const BufSlow& _other);

    std::string         skin;
    float               power;

    float               passedTime;

    AnimatedUnitModel*  bufedUnit;
    AnimatedUnitModel*  model;
    
private:
    float               difference;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufSlow__) */
