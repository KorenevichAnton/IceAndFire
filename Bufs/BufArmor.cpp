#include "BufArmor.h"
#include "Settings.h"
#include "Utils.h"
#include <stdio.h>
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufArmor::BufArmor():
Buf(),
skin(""),
power(0),
passedTime(0),
modelShowTime(0),
bufedUnit(NULL),
model(NULL),
difference(0)
{
    type = BUF_ARMOR;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufArmor::BufArmor(const BufArmor& _other):
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
    model = NULL;
    
    difference = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufArmor::~BufArmor()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufArmor* BufArmor::createTS(BattleModel* _battleModel, int _bufID)
{
    BufArmor* pBuf = new BufArmor();
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
void BufArmor::initWithXML()
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
BufArmor* BufArmor::copy() const
{
    return new BufArmor(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufArmor::activate(AnimatedUnitModel* _unit)
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
void BufArmor::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;

        if(bufedUnit)
            bufedUnit->setReduce(bufedUnit->getReduce() - difference);
        
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
void BufArmor::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
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
        if(!bufedUnit)
        {
            bufedUnit = _unit;
            
            float previous = _unit->getReduce();
            _unit->setReduce(previous + power);
            difference = _unit->getReduce() - previous;
        }
    }
    else
    {
        disactivate();
    }

    passedTime += _dt;
}

//---------------------------------------------------------------------------------------------------------------------------------
