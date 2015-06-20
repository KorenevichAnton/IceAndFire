#include "BufPush.h"
#include "Settings.h"
#include "Utils.h"
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufPush::BufPush():
Buf(),
skin(""),
speed(0),
distance(0),
passedTime(0),
model(NULL)
{
    type = BUF_PUSH;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPush::BufPush(const BufPush& _other):
Buf(_other)
{
    pBattleModel = _other.pBattleModel;

    type = _other.type;
    state = BUF_NOT_ACTIVED;

    skin = _other.skin;
    speed = _other.speed;
    distance = _other.distance;

    passedTime = 0;
    
    model = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPush::~BufPush()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPush* BufPush::createTS(BattleModel* _battleModel, int _bufID)
{
    BufPush* pBuf = new BufPush();
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
void BufPush::initWithXML()
{
    const XMLNode& mainNode = Utils::getNodeFormXMLFile("bufs.xml", "bufs");
    for (int i = 0; i < mainNode.nChildNode("buf"); i++)
    {
        const XMLNode& bufNode = mainNode.getChildNode("buf", i);
        if(id == Utils::readInt(bufNode, "id"))
        {
            chance = Utils::readFloat(bufNode, "chance");
            info = Utils::readString(bufNode, "info");
            color = (cocos2d::ccColor3B){100, 200, 100};

            if(Utils::readString(bufNode, "skin") != "")
                skin = CCString::createWithFormat( "Textures/Skins/bufs/%s", Utils::readString(bufNode, "skin").c_str() )->getCString();
            
            speed = Utils::readFloat(bufNode, "speed");
            distance = Utils::readFloat(bufNode, "distance");
            time = distance / speed;

            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPush* BufPush::copy() const
{
    return new BufPush(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufPush::activate(AnimatedUnitModel* _unit)
{
    if(state == BUF_NOT_ACTIVED)
    {
        if(rand()%100 < chance)
        {
            state = BUF_ACTIVATED;

            team = (_unit->getTeam() == 1) ? 2 : 1;
            line = _unit->getLine();
            
            
            //create model
            model = new AnimatedUnitModel();
            model->setType(AUM_TYPE_SPELL);
            model->setSkin(skin);
            model->setOriginSkinPoint(ccp(0, 0));
            model->setState("wait1");
            model->setAnimationSpeed(10);
            
            cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, model, 0);
            pBattleModel->addCommand(add);
            add->release();
            
            model->release();
            
            
            //buf unit
            time = distance / speed;
            
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
void BufPush::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;
        
        if(model)
        {
            cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, model, 0);
            pBattleModel->addCommand(rem);
            rem->release();
        }
        
        model = NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void BufPush::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(model)
        model->setPosition(_unit->getPosition());
    
    if(_unit->isAlive() && passedTime <= time)
    {
        float bfWidth = Settings::sharedInstance()->getBattleFieldRect().size.width;

        cocos2d::CCPoint newPos( _unit->getPosition().x + ((team == 1) ? (speed * FPS) : (-speed * FPS)),
                                _unit->getPosition().y );

        if(newPos.x < (bfWidth - BATTLE_ZONE_WIDTH) / 4)
            newPos.x = (bfWidth - BATTLE_ZONE_WIDTH) / 4;

        if(newPos.x > bfWidth - (bfWidth - BATTLE_ZONE_WIDTH) / 4)
            newPos.x = bfWidth - (bfWidth - BATTLE_ZONE_WIDTH) / 4;

        _unit->setPosition(newPos);
    }
    else
    {
        disactivate();
    }

    passedTime += _dt;
}

//---------------------------------------------------------------------------------------------------------------------------------
