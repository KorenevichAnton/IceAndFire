#ifndef __BattleMagic__BufSpeed__
#define __BattleMagic__BufSpeed__

#include "Buf.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufSpeed: public Buf
{
public:
    static BufSpeed* createTS(BattleModel* _battleModel, int _bufID);

    virtual bool    activate(AnimatedUnitModel* _activator);
    virtual void    update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float dt);
    virtual void    disactivate();

    BufSpeed*        copy() const;

    virtual ~BufSpeed();

protected:
    void            initWithXML();

    BufSpeed();
    BufSpeed(const BufSpeed& _other);

    std::string         skin;
    float               power;

    float               passedTime;
    float               modelShowTime;

    AnimatedUnitModel*  bufedUnit;
    AnimatedUnitModel*  model;
    
private:
    float               difference;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufSpeed__) */
