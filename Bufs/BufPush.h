#ifndef __BattleMagic__BufPush__
#define __BattleMagic__BufPush__

#include "Buf.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufPush: public Buf
{
public:
    static BufPush* createTS(BattleModel* _battleModel, int _bufID);

    virtual bool    activate(AnimatedUnitModel* _unit);
    virtual void    update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float dt);
    virtual void    disactivate();
    
    float           getSpeed() const { return speed; }
    void            setSpeed(float _speed) { speed = _speed; }
    
    float           getDistance() const { return distance; }
    void            setDistance(float _distance) { distance = _distance; }
    
    BufPush*        copy() const;

    virtual ~BufPush();

protected:
    void            initWithXML();

    BufPush();
    BufPush(const BufPush& _other);

    std::string         skin;
    float               speed;
    float               distance;

    float               passedTime;
    
    AnimatedUnitModel*  model;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufPush__) */
