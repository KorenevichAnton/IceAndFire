#include "BufPoison.h"
#include "Settings.h"
#include "Utils.h"
#include <stdio.h>
#include "BattleModel.h"
#include "cmdAddUnit.h"
#include "cmdRemoveUnit.h"
#include "cmdAttack.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BufPoison::BufPoison():
Buf(),
skin(""),
damage(0),
cooldown(0),
remainedCoolDown(0),
passedTime(0),
model(NULL)
{
    type = BUF_POISON;
    state = BUF_NOT_ACTIVED;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPoison::BufPoison(const BufPoison& _other):
Buf(_other)
{
    pBattleModel = _other.pBattleModel;

    type = _other.type;
    state = BUF_NOT_ACTIVED;

    skin = _other.skin;
    damage = _other.damage;
    cooldown = _other.cooldown;
    remainedCoolDown = 0;
    passedTime = 0;

    model = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPoison::~BufPoison()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPoison* BufPoison::createTS(BattleModel* _battleModel, int _bufID)
{
    BufPoison* pBuf = new BufPoison();
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
void BufPoison::initWithXML()
{
    const XMLNode& mainNode = Utils::getNodeFormXMLFile("bufs.xml", "bufs");
    for(int i = 0; i < mainNode.nChildNode("buf"); i++)
    {
        const XMLNode& bufNode = mainNode.getChildNode("buf", i);
        if(id == Utils::readInt(bufNode, "id"))
        {
            chance = Utils::readFloat(bufNode, "chance");
            time = Utils::readFloat(bufNode, "time");
            info = Utils::readString(bufNode, "info");
            color = (cocos2d::ccColor3B){100, 255, 100};
            
            if(Utils::readString(bufNode, "skin") != "")
                skin = CCString::createWithFormat( "Textures/Skins/bufs/%s", Utils::readString(bufNode, "skin").c_str() )->getCString();
            
            damage = Utils::readFloat(bufNode, "damage");
            cooldown = Utils::readFloat(bufNode, "cooldown");

            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
BufPoison* BufPoison::copy() const
{
    return new BufPoison(*this);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BufPoison::activate(AnimatedUnitModel* _unit)
{
    if(state == BUF_NOT_ACTIVED)
    {
        if(rand()%100 < chance)
        {
            state = BUF_ACTIVATED;

            team = (_unit->getTeam() == 1) ? 2 : 1;
            line = _unit->getLine();

            
            //create attack
            UnitAttack* attack = new UnitAttack();
            attack->priority = 0;
            attack->type = AT_ENEMY;
            attack->schoolName = "";
            attack->damage = damage;
            attack->dmgColor = color;

            //create model
            model = new AnimatedUnitModel();
            model->setType(AUM_TYPE_SPELL);
            model->setSkin(skin);
            model->setOriginSkinPoint(ccp(0, 0));
            model->setState("wait1");
            model->setAnimationSpeed(10);
            model->addAttack(attack);
            
            attack->release();

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
void BufPoison::disactivate()
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
void BufPoison::update(AnimatedUnitModel* _unit, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(model)
        model->setPosition(_unit->getPosition());

    if(_unit->isAlive() && passedTime <= time)
    {
        remainedCoolDown -= _dt;

        if(remainedCoolDown <= 0 && model)
        {
            cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, model, _unit, model->getAttack(), 0);
            pBattleModel->addCommand(cmdAtt);
            cmdAtt->release();

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
