#include "BufSlow.h"
#include "Settings.h"
#include "Utils.h"
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufSlow::BufSlow():
Buf(),
skin(""),
power(0),
passedTime(0),
bufedUnit(NULL),
model(NULL),
difference(0)
{
    type = BUF_SLOW;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufSlow::BufSlow(const BufSlow& _other):
Buf(_other)
{
    pBattleModel = _other.pBattleModel;
    
    type = _other.type;
    state = BUF_NOT_ACTIVED;
    
    skin = _other.skin;
    power = _other.power;
    
    passedTime = 0;
    
    bufedUnit = NULL;
    model = NULL;
    
    difference = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufSlow::~BufSlow()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufSlow* BufSlow::createTS(BattleModel* _battleModel, int _bufID)
{
    BufSlow* pBuf = new BufSlow();
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
void BufSlow::initWithXML()
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
            
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
BufSlow* BufSlow::copy() const
{
    return new BufSlow(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufSlow::activate(AnimatedUnitModel* _unit)
{
    if(state == BUF_NOT_ACTIVED)
    {
        if(rand()%100 < chance)
        {
            state = BUF_ACTIVATED;
            
            team = (_unit->getTeam() == 1) ? 2 : 1;
            line = _unit->getLine();
            
            
            //create model
            if(_unit->getType() == AUM_TYPE_MOB)
            {
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
            }
            
            
            //buf unit
            bufedUnit = _unit;
            
            float previous = _unit->getMovingSpeed();
            _unit->setMovingSpeed(previous - previous * power * 0.01);
            difference = _unit->getMovingSpeed() - previous;
            
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
void BufSlow::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;
        
        if(bufedUnit)
            bufedUnit->setMovingSpeed(bufedUnit->getMovingSpeed() - difference);
        
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
void BufSlow::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(model)
        model->setPosition(_unit->getPosition());

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
