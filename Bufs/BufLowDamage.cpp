#include "BufLowDamage.h"
#include "Settings.h"
#include "Utils.h"
#include <stdio.h>
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufLowDamage::BufLowDamage():
Buf(),
skin(""),
power(0),
passedTime(0),
bufedUnit(NULL),
model(NULL)
{
    type = BUF_LOW_DAMAGE;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLowDamage::BufLowDamage(const BufLowDamage& _other):
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
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLowDamage::~BufLowDamage()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufLowDamage* BufLowDamage::createTS(BattleModel* _battleModel, int _bufID)
{
    BufLowDamage* pBuf = new BufLowDamage();
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
void BufLowDamage::initWithXML()
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
BufLowDamage* BufLowDamage::copy() const
{
    return new BufLowDamage(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufLowDamage::activate(AnimatedUnitModel* _unit)
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
            bufedUnit = _unit;

            CCObject* obj;
            CCARRAY_FOREACH(bufedUnit->_attacks, obj)
            {
                UnitAttack* attModel = (UnitAttack*)obj;
                if(attModel->type == AT_ENEMY)
                {
                    //reduce attack damage
                    attModel->damage -= attModel->damage * power * 0.01;
                    attModel->damageDelta -= attModel->damageDelta * power * 0.01;
                }
            }

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
void BufLowDamage::disactivate()
{
    if(state == BUF_ACTIVATED)
    {
        state = BUF_DISACTIVATED;

        if(bufedUnit)
        {
            //backdown all attacks
            CCObject* obj;
            CCARRAY_FOREACH(bufedUnit->_attacks, obj)
            {
                UnitAttack* attModel = (UnitAttack*)obj;
                if(attModel->type == AT_ENEMY)
                {
                    attModel->damage += (attModel->damage / (100 - power)) * 100;
                    attModel->damageDelta += (attModel->damageDelta / (100 - power)) * 100;
                }
            }
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
void BufLowDamage::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
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
