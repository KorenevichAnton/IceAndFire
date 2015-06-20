#include "Buf.h"
#include "xmlParser.h"
#include "Utils.h"
#include "BufAll.h"
#include "BattleModel.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
Buf::Buf():
UnitAI(),
id(0),
type(BUF_EMPTY),
kind(BUF_SEND),
state(BUF_NOT_ACTIVED),
chance(1),
time(0),
info(""),
color((cocos2d::ccColor3B){255, 255, 255}),
team(0),
line(0)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
Buf::Buf(const Buf& _other)
{
    //pBattleModel = _other.pBattleModel;
    
    id = _other.id;
    type = _other.type;
    kind = _other.kind;
    state = BUF_NOT_ACTIVED;
    chance = _other.chance;
    time = _other.time;
    info = _other.info;
    color = _other.color;

    team = _other.team;
    line = _other.line;
}

//---------------------------------------------------------------------------------------------------------------------------------
Buf::~Buf()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
Buf* Buf::createTS(BattleModel* _battleModel, int _bufID)
{
    Buf* pBuf = NULL;

    const XMLNode& mainNode = Utils::getNodeFormXMLFile("bufs.xml", "bufs");
    for(int i = 0; i < mainNode.nChildNode("buf"); i++)
    {
        const XMLNode& bufNode = mainNode.getChildNode("buf", i);
        if(_bufID == Utils::readInt(bufNode, "id"))
        {
            string bufType = Utils::readString(bufNode, "type");

            if(bufType == "FireSword")
                pBuf = BufFireSword::createTS(_battleModel, _bufID);
            if(bufType == "Fire")
                pBuf = BufFire::createTS(_battleModel, _bufID);
            if(bufType == "Armor")
                pBuf = BufArmor::createTS(_battleModel, _bufID);
            if(bufType == "Freeze")
                pBuf = BufFreeze::createTS(_battleModel, _bufID);
            if(bufType == "Slow")
                pBuf = BufSlow::createTS(_battleModel, _bufID);
            if(bufType == "Push")
                pBuf = BufPush::createTS(_battleModel, _bufID);
            if(bufType == "LifePower")
                pBuf = BufLifePower::createTS(_battleModel, _bufID);
            if(bufType == "Speed")
                pBuf = BufSpeed::createTS(_battleModel, _bufID);
            if(bufType == "LowDamage")
                pBuf = BufLowDamage::createTS(_battleModel, _bufID);
            if(bufType == "Regeneration")     
                pBuf = BufRegeneration::createTS(_battleModel, _bufID);
            if(bufType == "Resurrection")
                pBuf = BufResurrection::createTS(_battleModel, _bufID);
            if(bufType == "Poison")
                pBuf = BufPoison::createTS(_battleModel, _bufID);
            /* */
            break;
        }
    }

    return pBuf;
}

//---------------------------------------------------------------------------------------------------------------------------------
