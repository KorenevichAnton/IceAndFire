#ifndef __BattleMagic__BufLifePower__
#define __BattleMagic__BufLifePower__

#include "Buf.h"
#include <vector>

class AnimatedUnitModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BufLifePower: public Buf
{
public:
    static BufLifePower*   createTS(BattleModel* _battleModel, int _bufID);

    virtual bool            activate(AnimatedUnitModel* _unit);
    virtual void            update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt);
    virtual void            disactivate();

    BufLifePower*           copy() const;

    virtual ~BufLifePower();

protected:
    BufLifePower();
    BufLifePower(const BufLifePower& _other);

    void                    initWithXML();

    std::string                 skin;
    float                       power;

    float                       passedTime;
    float                       modelShowTime;

    AnimatedUnitModel*          bufedUnit;
    AnimatedUnitModel*          model;
    
private:
    float                   difference;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BufLifePower__) */
