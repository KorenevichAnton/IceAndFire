//
//  PassiveSpellAI.cpp
//  BattleMagic
//
//  Created by madmin on 12/24/13.
//
//

#include "PassiveSpellAI.h"

#include "UnitActionHidenDamage.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAll.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
PassiveSpellAI::PassiveSpellAI():UnitAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
PassiveSpellAI::~PassiveSpellAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
PassiveSpellAI* PassiveSpellAI::createTS(BattleModel *commandsQueue)
{
    PassiveSpellAI* pRet = new PassiveSpellAI();
    if(pRet)
    {
        pRet->_commands = commandsQueue;
        CC_SAFE_RETAIN(commandsQueue);
    }
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void PassiveSpellAI::update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits, float dt)
{
    //unitAI->setPosition(ccpAdd(unitAI->getPosition(), ccpMult(unitAI->getMoveDirection(),unitAI->getMovingSpeed()*dt)));
    CCObject* obj = NULL;
    CCARRAY_FOREACH(allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        if  ((unit->isAlive()) &&
            (unit->getTeam() == unitAI->getTeam()) &&
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