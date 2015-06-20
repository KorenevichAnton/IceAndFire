//
//  BulletUnitAI.cpp
//  BattleMagic
//
//  Created by Andrey Ryabushkin on 15.03.13.
//
//

#include "BulletUnitAI.h"
#include "Settings.h"
#include "cmdRemoveUnit.h"
#include "BattleModel.h"
#include "spellStatCounter.h"
//#include "BufSpeed.h"
#include "Buf.h"
#include "cmdAll.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
BulletUnitAI::BulletUnitAI():UnitAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
BulletUnitAI::~BulletUnitAI()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
BulletUnitAI* BulletUnitAI::createTS(BattleModel *commandsQueue)
{
    BulletUnitAI* pRet = new BulletUnitAI();
    if(pRet)
    {
        pRet->_commands = commandsQueue;
        CC_SAFE_RETAIN(commandsQueue);
    }
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BulletUnitAI::update(AnimatedUnitModel* unitAI, const cocos2d::CCArray* allUnits, float dt)
{
    CCObject* obj = NULL;
    //fucking remove spell
    
    if(unitAI->getSpecialID() == 12 && unitAI->getType() == AUM_TYPE_SPELL)
    {
        CCARRAY_FOREACH(allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            cmdAttack* cmdAtt = cmdAttack::createTS(unitAI, unit, unitAI->getAttack());
            _commands->addCommand(cmdAtt);
            cmdAtt->release();
        }
    }
     
    if (unitAI->isAlive())
    {
        unitAI->setPosition(ccpAdd(unitAI->getPosition(), ccpMult(unitAI->getMoveDirection(),unitAI->getMovingSpeed()*dt)));
        
        if((unitAI->getType() == AUM_TYPE_SPELL) &&
           (unitAI->getSpecialID() == 6 || unitAI->getSpecialID() == 12 || unitAI->getSpecialID() == 9 || unitAI->getSpecialID() == 15) &&
           (unitAI->lifeTimeStarted <= time(NULL)))
        {
            //fucking remove slowing
            if(unitAI->getSpecialID() == 15)
            {
                CCARRAY_FOREACH(allUnits, obj)
                {
                    AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
                    //acceleration after blizzard
                    if((unit->slowed == 1) &&
                       (unitAI->getType() == AUM_TYPE_SPELL) &&
                       (unitAI->getSpecialID() == 15) &&
                       (unitAI->getTeam() != unit->getTeam()) &&
                       (unitAI->getLine() == unit->getLine()))
                        //))
                    {
                        if(unit->getType() == AUM_TYPE_MOB )
                        {
                            unit->setMovingSpeed((unit->getMovingSpeed() * 100 / 50));
                            unit->slowed = 0;
                        }
                        if(unit->getType() == AUM_TYPE_BULLET)
                        {
                            UnitAttack* att = unit->getAttack();
                            unit->getLine();
                            unit->setMovingSpeed((unit->getMovingSpeed() * 100 / 50));
                            att->damage = att->damage + att->damage * 100 * 0.01;
                            unit->addAttack(att);
                            unit->slowed = 0;
                        }
                    }
                }
            }

            cmdRemoveUnit* cmdrm = cmdRemoveUnit::createTS(_commands, unitAI, 0);
            _commands->addCommand(cmdrm);
            cmdrm->release();
        }

        
        CCARRAY_FOREACH(allUnits, obj)
        {
            
            int priority = 0;   //priority of possible attack
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            //fucking slowing by blizzard
            if((unit->slowed == 0) &&
               (unitAI->getType() == AUM_TYPE_SPELL) &&
               (unitAI->getSpecialID() == 15) &&
               (unitAI->lifeTimeStarted > time(NULL)) &&
               (unitAI->getTeam() != unit->getTeam()) &&
               (unitAI->getLine() == unit->getLine()))
               //))
            {
                if(unit->getType() == AUM_TYPE_MOB)
                {
                    unit->setMovingSpeed(unit->getMovingSpeed() - (unit->getMovingSpeed() * 50 * 0.01));
                    unit->slowed = 1;
                }
                if(unit->getType() == AUM_TYPE_BULLET)
                {
                    UnitAttack* att = unit->getAttack();
                    unit->getLine();
                    unit->setMovingSpeed(unit->getMovingSpeed() - (unit->getMovingSpeed() * 50 * 0.01));
                    att->damage = att->damage - att->damage * 50 * 0.01;
                    unit->addAttack(att);
                    unit->slowed = 1;
                }
            }

            
            if ((_exeptionList.count(unit->getID()) == 0) &&
                (unit->isAlive()) &&
                ((unitAI->getType()==AUM_TYPE_SPELL && unitAI->getSpecialID() == 9 && unit->getTeam() == unitAI->getTeam()) ||
                 (unitAI->getType()==AUM_TYPE_SPELL && unitAI->getSpecialID() == 12 && unit->getTeam() == unitAI->getTeam()) ||
                 (unit->getTeam() != unitAI->getTeam() && unitAI->getSpecialID() != 12) )&&
                (unit->getType() == AUM_TYPE_MOB) &&
                (UnitAI::canAttack(unitAI, unit, priority) >= 0) &&
                (!Settings::sharedInstance()->getFriedlyRect().containsPoint(unitAI->getPosition())))
            {
                cmdAttack* cmdAtt = cmdAttack::createTS(unitAI, unit, unitAI->getAttack());
                _commands->addCommand(cmdAtt);
                cmdAtt->release();
                
                  
                if(unitAI->getSpecialID() != 6 && unitAI->getSpecialID() != 11 && unitAI->getSpecialID() != 10 && unitAI->getSpecialID() != 12 && unitAI->getSpecialID() != 9)
                {
                    cmdRemoveUnit* rem = cmdRemoveUnit::createTS((BattleModel*)_commands, unitAI, 0);
                    _commands->addCommand(rem);
                    rem->release();
                }
                
                // statistic collector
                spellStatCounter::hit(unitAI->getID());
                break;
            }else if(!unit->isAlive() &&
                     unit->getTeam() != unitAI->getTeam() &&
                     unit->getType() == AUM_TYPE_MOB &&
                     unit->maxRestore > 0 &&
                     (UnitAI::canAttack(unitAI, unit, priority) >= 0) &&
                     unitAI->getAttack()->schoolName == "fire")
            {
                    unit->maxRestore = 0;
                    unit->restoreHp = 0;
                    unit->accumulationRestoreHp = 0;
                
                    cmdRemoveUnit* rem = cmdRemoveUnit::createTS((BattleModel*)_commands, unitAI, 0);
                    _commands->addCommand(rem);
                    rem->release();

                    // statistic collector
                    spellStatCounter::hit(unitAI->getID());
                    break;
            }
        }
        
        // border controll
        if (!Settings::sharedInstance()->getDeathRect().containsPoint(unitAI->getPosition()))
        {
            cmdRemoveUnit* rem = cmdRemoveUnit::createTS((BattleModel*)_commands, unitAI, 0);
            _commands->addCommand(rem);
            rem->release();
         
            // statistic collector
            spellStatCounter::miss(unitAI->getID());
        } 
    }
    else
    {
        cmdRemoveUnit* rem = cmdRemoveUnit::createTS((BattleModel*)_commands, unitAI, 0);
        _commands->addCommand(rem);
        rem->release();
    }
    
}

//---------------------------------------------------------------------------------------------------------------------------------