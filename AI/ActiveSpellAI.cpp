//
//  ActiveSpellAI.cpp
//  BattleMagic
//
//  Created by madmin on 12/25/13.
//
//

#include "ActiveSpellAI.h"

#include "UnitActionHidenDamage.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAll.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
ActiveSpellAI::ActiveSpellAI():UnitAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
ActiveSpellAI::~ActiveSpellAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
ActiveSpellAI* ActiveSpellAI::createTS(BattleModel *commandsQueue)
{
    ActiveSpellAI* pRet = new ActiveSpellAI();
    if(pRet)
    {
        pRet->_commands = commandsQueue;
        CC_SAFE_RETAIN(commandsQueue);
    }
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void ActiveSpellAI::update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits, float dt)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        if  ((unit->isAlive()) &&
             (unit->getTeam() != unitAI->getTeam()) &&
             (unit->getLine() == unitAI->getLine()) &&
             (unit->getType() == AUM_TYPE_MOB))
        {
            cmdAttack* cmdAtt = cmdAttack::createTS(unitAI, unit, unitAI->getAttack());
            _commands->addCommand(cmdAtt);
            cmdAtt->release();
        }
    }
    
    cmdRemoveUnit* rem = cmdRemoveUnit::createTS((BattleModel*)_commands, unitAI, 0);
    _commands->addCommand(rem);
    rem->release();
    
}

//---------------------------------------------------------------------------------------------------------------------------------