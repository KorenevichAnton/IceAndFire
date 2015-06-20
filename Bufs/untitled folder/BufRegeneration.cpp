#include "BufRegeneration.h"
#include "Settings.h"
#include "Utils.h"
#include "BattleModel.h"

using namespace std;
using namespace XMLParser;

//---------------------------------------------------------------------------------------------------------------------------------
BufRegeneration::BufRegeneration():
Buf(),
skin(""),
power(0),
cooldown(0),
remainedCoolDown(0),
passedTime(0),
bufedUnit(NULL)
{
    type = BUF_REGENERATION;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufRegeneration::BufRegeneration(const BufRegeneration& _other):
Buf(_other)
{
    pBattleModel = _other.pBattleModel;

    type = _other.type;
    state = BUF_NOT_ACTIVED;

    skin = _other.skin;
    power = _other.power;

    cooldown = _other.cooldown;
    remainedCoolDown = 0;
    passedTime = 0;

    bufedUnit = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufRegeneration::~BufRegeneration()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufRegeneration* BufRegeneration::createTS(BattleModel* _battleModel, int _bufID)
{
    BufRegeneration* pBuf = new BufRegeneration();
    if(pBuf)
    {
        pBuf->pBattleModel = _battleModel;
        pBuf->id = _bufID;

        pBuf->initWithXML();

        return pBuf;
    }
    else
    {
        delete pBuf;
        pBuf = NULL;
        return NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void BufRegeneration::initWithXML()
{
    const XMLNode& mainNode = Utils::getNodeFormXMLFile("bufs.xml", "bufs");
    for (int i = 0; i < mainNode.nChildNode("buf"); i++)
    {
        const XMLNode& bufNode = mainNode.getChildNode("buf", i);
        if(id == Utils::readInt(bufNode, "id"))
        {
            chance = Utils::readFloat(bufNode, "chance");
            time = Utils::readFloat(bufNode, "time");
            info = Utils::readString(bufNode, "info");
            color = (cocos2d::ccColor3B){100, 100, 200};

            skin = Utils::readString(bufNode, "skin");
            power = Utils::readFloat(bufNode, "power");
            cooldown = Utils::readFloat(bufNode, "cooldown");

            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
BufRegeneration* BufRegeneration::copy() const
{
    return new BufRegeneration(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufRegeneration::activate(AnimatedUnitModel* _unit)
{
    if(state == BUF_NOT_ACTIVED)
    {
        if(rand()%100 < chance)
        {
            state = BUF_ACTIVATED;

            team = _unit->getTeam();
            line = _unit->getLine();

            bufedUnit = _unit;

            return true;
        }
        else
        {
            disactivate();
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BufRegeneration::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;

        if(bufedUnit)
        {
            if(!bufedUnit->getAlive())
                bufedUnit->setHp(0);
        }

        bufedUnit = NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void BufRegeneration::update(AnimatedUnitModel* _unit, const vector<AnimatedUnitModel*>& _allUnits, float _dt)
{
    if(passedTime <= time)
    {
        remainedCoolDown -= _dt;

        if(remainedCoolDown <= 0)
        {
            if((_unit->getHp() + _unit->getMaxHp() * power * 0.01) >= _unit->getMaxHp())
                _unit->setHp( _unit->getMaxHp() );
            else
                _unit->setHp( _unit->getHp() + _unit->getMaxHp() * power * 0.01 );

            remainedCoolDown = cooldown;
        }
    }
    else
    {
        disactivate();
    }

    passedTime += _dt;
}

//---------------------------------------------------------------------------------------------------------------------------------
