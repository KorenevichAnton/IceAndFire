#include "BufResurrection.h"
#include "Settings.h"
#include "Utils.h"
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufResurrection::BufResurrection():
Buf(),
skin(""),
power(0),
threshold(0),
cooldown(0),
remainedCoolDown(0),
passedTime(0),
bufedUnit(NULL),
model(NULL)
{
    type = BUF_RESURRECTION;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufResurrection::BufResurrection(const BufResurrection& _other):
Buf(_other)
{
    pBattleModel = _other.pBattleModel;

    type = _other.type;
    state = BUF_NOT_ACTIVED;

    skin = _other.skin;
    power = _other.power;
    threshold = _other.threshold;

    cooldown = _other.cooldown;
    remainedCoolDown = 0;
    passedTime = 0;

    bufedUnit = NULL;
    model = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufResurrection::~BufResurrection()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufResurrection* BufResurrection::createTS(BattleModel* _battleModel, int _bufID)
{
    BufResurrection* pBuf = new BufResurrection();
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
void BufResurrection::initWithXML()
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

            if(Utils::readString(bufNode, "skin") != "")
                skin = CCString::createWithFormat( "Textures/Skins/bufs/%s", Utils::readString(bufNode, "skin").c_str() )->getCString();
            
            power = Utils::readFloat(bufNode, "power");
            threshold = Utils::readFloat(bufNode, "threshold");
            cooldown = Utils::readFloat(bufNode, "cooldown");

            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
BufResurrection* BufResurrection::copy() const
{
    return new BufResurrection(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufResurrection::activate(AnimatedUnitModel* _unit)
{
    if(state == BUF_NOT_ACTIVED && _unit->getDeathTime() > 0)
    {
        if(rand()%100 < chance)
        {
            state = BUF_ACTIVATED;

            team = _unit->getTeam();
            line = _unit->getLine();
            
            
            //create model
            model = new AnimatedUnitModel();
            model->setType(AUM_TYPE_SPELL);
            model->setSkin(skin);
            model->setOriginSkinPoint(ccp(0, -50));
            model->setState("wait1");
            model->setAnimationSpeed(10);
            
            cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, model, 0);
            pBattleModel->addCommand(add);
            add->release();
            
            model->release();
            
            
            //buf unit
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
void BufResurrection::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;

        if(bufedUnit)
        {
            if(!bufedUnit->isAlive())
                bufedUnit->setHp(0);
        }

        bufedUnit = NULL;
        
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
void BufResurrection::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(model)
        model->setPosition(_unit->getPosition());
    
    if(!_unit->isAlive() && passedTime <= time)
    {
        remainedCoolDown -= _dt;

        if(remainedCoolDown <= 0)
        {
            if((_unit->getHp() + _unit->getMaxHp() * power * 0.01) >= _unit->getMaxHp())
                _unit->setHp( _unit->getMaxHp() );
            else
                _unit->setHp( _unit->getHp() + _unit->getMaxHp() * power * 0.01 );

            //resurrect!
            if(_unit->getHp() / _unit->getMaxHp() >= threshold * 0.01 )
            {
                _unit->setAlive(true);
                _unit->setState("wait1");
            }

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
