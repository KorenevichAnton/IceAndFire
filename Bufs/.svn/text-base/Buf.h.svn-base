#ifndef __BattleMagic__Buf__
#define __BattleMagic__Buf__

#include "UnitAI.h"
#include "xmlParser.h"
#include <iostream>

class BufCondition;
class UnitAttack;

//---------------------------------------------------------------------------------------------------------------------------------
enum bufType
{
    BUF_EMPTY = 0,
    BUF_FIRE = 1,
    BUF_FIRE_SWORD = 2,
    BUF_PUSH = 3,
    BUF_SLOW = 4,
    BUF_FREEZE = 5,
    BUF_ARMOR = 6,
    BUF_LIFE_POWER = 7,
    BUF_SPEED = 8,
    BUF_LOW_DAMAGE = 9,
    BUF_REGENERATION = 10,
    BUF_RESURRECTION = 11,
    BUF_POISON = 12
};

enum bufKind
{
    BUF_SEND = 0,
    BUF_SELF = 1
};

enum bufState
{
    BUF_NOT_ACTIVED = 0,
    BUF_ACTIVATED = 1,
    BUF_DISACTIVATED = 2
};

//---------------------------------------------------------------------------------------------------------------------------------
class Buf: public UnitAI
{
public:
    static Buf*                 createTS(BattleModel* _battleModel, int _bufID);

    virtual bool                activate(AnimatedUnitModel* _activator) = 0;
    virtual void                update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt) = 0;
    virtual void                disactivate() = 0;


    virtual int                 getID() const { return id; }

    virtual bufType             getType() const { return type; }
    virtual void                setType(bufType _type){ type = _type; }

    virtual bufKind             getKind() const { return kind; }
    virtual void                setKind(bufKind _kind){ kind = _kind; }

    virtual bufState            getState() const { return state; }
    virtual void                setState(bufState _state) { state = _state; }

    virtual float               getChance() const { return chance; }
    virtual void                setChance(float _chance) { chance = _chance; }

    virtual float               getTime() const { return time; }
    virtual void                setTime(float _time) { time = _time; }

    virtual std::string         getInfo() const { return info; }
    virtual void                setInfo(std::string _info) { info = _info; }

    virtual cocos2d::ccColor3B  getColor() const { return color; }
    virtual void                setColor(cocos2d::ccColor3B _color) { color = _color; }


    virtual int                 getTeam() const { return team; }
    virtual void                setTeam(int _team){ team = _team; }

    virtual int                 getLine() const { return line; }
    virtual void                setLine(int _line){ line = _line; }


    virtual Buf*                copy() const = 0;
    virtual ~Buf();

protected:
    int                         id;
    bufType                     type;
    bufKind                     kind;
    bufState                    state;
    float                       chance;
    float                       time;
    std::string                 info;
    cocos2d::ccColor3B          color;

    int                         team;
    int                         line;

    Buf();
    Buf(const Buf& other);
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__Buf__) */
