#ifndef __BattleMagic__BufFreeze__
#define __BattleMagic__BufFreeze__

#include "Buf.h"

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufFreeze: public Buf
{
public:
    static BufFreeze*   createTS(BattleModel* _battleModel, int _bufID);

    virtual bool        activate(AnimatedUnitModel* _unit);
    virtual void        update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float dt);
    virtual void        disactivate();

    BufFreeze*          copy() const;

    virtual ~BufFreeze();

protected:
    void            initWithXML();

    BufFreeze();
    BufFreeze(const BufFreeze& _other);

    std::string         skin;

    float               passedTime;

    AnimatedUnitModel*  bufedUnit;
    float               prevSpeed;

    AnimatedUnitModel*  model;
    
private:
    float               difference;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufFreeze__) */
