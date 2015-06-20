#include "BufLifePower.h"
#include "Settings.h"
#include "Utils.h"
#include <stdio.h>
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufLifePower::BufLifePower():
Buf(),
skin(""),
power(0),
passedTime(0),
modelShowTime(0),
bufedUnit(NULL),
model(NULL),
difference(0)
{
    type = BUF_LIFE_POWER;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLifePower::BufLifePower(const BufLifePower& _other):
Buf(_other)
{
    pBattleModel = _other.pBattleModel;

    type = _other.type;
    state = BUF_NOT_ACTIVED;

    skin = _other.skin;
    power = _other.power;
    
    passedTime = 0;
    modelShowTime = 0;
    
    bufedUnit = NULL;
    model = 0;
    
    difference = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLifePower::~BufLifePower()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLifePower* BufLifePower::createTS(BattleModel* _battleModel, int _bufID)
{
    BufLifePower* pBuf = new BufLifePower();
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
void BufLifePower::initWithXML()
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
            color = (cocos2d::ccColor3B){200, 100, 100};

            if(Utils::readString(bufNode, "skin") != "")
                skin = CCString::createWithFormat( "Textures/Skins/bufs/%s", Utils::readString(bufNode, "skin").c_str() )->getCString();
            
            power = Utils::readFloat(bufNode, "power");

            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLifePower* BufLifePower::copy() const
{
    return new BufLifePower(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufLifePower::activate(AnimatedUnitModel* _unit)
{
    if(state == BUF_NOT_ACTIVED)
    {
        if(rand()%100 < chance)
        {
            state = BUF_ACTIVATED;

            team = _unit->getTeam();
            line = _unit->getLine();
            
            modelShowTime = 1.0;
            
            
            //create model
            model = new AnimatedUnitModel();
            model->setType(AUM_TYPE_SPELL);
            model->setSkin(skin);
            model->setOriginSkinPoint(ccp(0, 75));
            model->setState("wait1");
            model->setAnimationSpeed(10);
            
            cmdAddUnit* add = cmdAddUnit::createTS(pBattleModel, model, 0);
            pBattleModel->addCommand(add);
            add->release();
            
            model->release();

            
            //buf unit
            bufedUnit = _unit;
            
            float previous = _unit->getMaxHp();
            _unit->setMaxHp(previous + previous * power * 0.01);
            difference = _unit->getMaxHp() - previous;

            _unit->setHp(_unit->getMaxHp());
            
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
void BufLifePower::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;

        if(bufedUnit)
        {
            bufedUnit->setMaxHp(bufedUnit->getMaxHp() - difference);
            
            if(bufedUnit->getHp() > bufedUnit->getMaxHp())
                bufedUnit->setHp( bufedUnit->getMaxHp() );
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
void BufLifePower::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(model)
    {
        model->setPosition(_unit->getPosition());
        
        if(passedTime >= modelShowTime)
        {
            cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, model, 0);
            pBattleModel->addCommand(rem);
            rem->release();
            
            model = NULL;
        }
    }
    
    if(_unit->isAlive() && passedTime <= time)
    {
    }
    else
    {
        disactivate();
    }

    passedTime += _dt;
}

//---------------------------------------------------------------------------------------------------------------------------------
