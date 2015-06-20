//
//  FireSpellAI.cpp
//  BattleMagic
//
//  Created by Andrey Ryabushkin on 23.02.13.
//
//

#include "FireSpellAI.h"
#include "UnitActionHidenDamage.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAll.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
FireSpellAI::FireSpellAI():UnitAI(),
_damagedUnitsList(NULL),
_manaSource(NULL)
{
    _damagedUnitsList = CCDictionary::create();
    CC_SAFE_RETAIN(_damagedUnitsList);
}

//---------------------------------------------------------------------------------------------------------------------------------
FireSpellAI::~FireSpellAI()
{
    CC_SAFE_RELEASE_NULL(_damagedUnitsList);
}

//---------------------------------------------------------------------------------------------------------------------------------
FireSpellAI* FireSpellAI::createTS(BattleModel *commandsQueue)
{
    FireSpellAI* pRet = new FireSpellAI();
    if (pRet)
    {
        pRet->_commands = commandsQueue;
        CC_SAFE_RETAIN(commandsQueue);
    }
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void FireSpellAI::update(AnimatedUnitModel* unitAI,const cocos2d::CCArray *allUnits, float dt)
{
    CCObject* obj;
    bool activeSpellUse = false;
    
    if (unitAI->isAlive())
    {
        CCARRAY_FOREACH(allUnits, obj)
        {
            int priority = 0;   //priority of possible attack
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
                
            if ((unit->isAlive())&&(unit->getTeam() != unitAI->getTeam()) &&
                (unit->getType() == AUM_TYPE_MOB) &&
                (UnitAI::canAttack(unitAI, unit, priority) >= 0) &&
                (!Settings::sharedInstance()->getFriedlyRect().containsPoint(unitAI->getPosition())))
            {
                float dmg  = unitAI->getAttack()->damage * dt;
                cmdAttack* cmdAtt = cmdAttack::createTS(unitAI, unit, unitAI->getAttack(), dmg, true);
                _commands->addCommand(cmdAtt);
                cmdAtt->release();
                activeSpellUse = true;
            }
        }
        
        if(!activeSpellUse)
        {
            float manaNeed = unitAI->getAttack()->manaCost*dt;
            if ((!_manaSource) || (!_manaSource->checkManaEnough(_spellSchoolName, manaNeed)))
            {
                unitAI->setHp(0);
            }
            else
            {
                _manaSource->setMana(_spellSchoolName, _manaSource->getMana(_spellSchoolName)->curValue - manaNeed);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------