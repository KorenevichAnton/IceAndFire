#include "ElementaryUnitAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdSetState.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"
#include "BulletUnitAI.h"
#include "HUDScreen.h"
#include "Buf.h"

#include "Card.h"
#include "HeroAI.h"
#include "MobFactory.h"
#include "Utils.h"

#include "Buf.h"
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
ElementaryUnitAI::ElementaryUnitAI():UnitAI(), countPush(0), remainingCountPush(0)
{
    //get settings
    _settings = Settings::sharedInstance();
    CC_ASSERT(_settings);
}

//---------------------------------------------------------------------------------------------------------------------------------
ElementaryUnitAI::~ElementaryUnitAI()
{
    
}
//---------------------------------------------------------------------------------------------------------------------------------
ElementaryUnitAI* ElementaryUnitAI::createTS(BattleModel *commandsQueue)
{
    ElementaryUnitAI* pRet = new ElementaryUnitAI();
    if (pRet)
    {
        pRet->_commands = commandsQueue;
        CC_SAFE_RETAIN(commandsQueue);
    }
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void ElementaryUnitAI::update(AnimatedUnitModel* unitAI, const cocos2d::CCArray *allUnits, float dt)
{
    //update unit AI actions
    
    for(int i = 0; i < unitAI->_attacks->count(); i++)
    {
        UnitAttack* attModel = (UnitAttack*)unitAI->_attacks->objectAtIndex(i);
    
        if(attModel->remainedColdDown > 0)
        {
            attModel->remainedColdDown -= dt;
            if(attModel->remainedColdDown < dt)
                attModel->remainedColdDown = 0;
        }
        
        if(attModel->remainedActionColdDown > 0)
        {
            attModel->remainedActionColdDown -= dt;
            if(attModel->remainedActionColdDown < dt)
                attModel->remainedActionColdDown = 0;
        }
    }
    
    //mob's behavior
    if(unitAI->isAlive())
    {
        if(unitAI->getState() != "deafened")
        {
            int priority = 0;                                       //priority of possible action
            float useful = canAction(unitAI, allUnits, priority);   //check the possibility of the action
            if (unitAI->getActionWaitTime() > 0 && unitAI->getAttack(priority) && unitAI->getAttack(priority)->type != AT_CALL)
            {
                unitAI->setState("wait1");
                unitAI->setActionWaitTime(unitAI->getActionWaitTime() - dt);
            }
            else
            {
                UnitAttack* attM = unitAI->getAttack(priority);
                //for escape from its landing zone (for team 1)
                if(unitAI->getTeam() == 1 &&
                   unitAI->getPosition().x < (_settings->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 2 &&
                   canMove(unitAI, allUnits))
                {
                    useful = -1;
                }
                //
                
                //for escape from its landing zone (for team 2)
                if(unitAI->getTeam() == 2 &&
                   unitAI->getPosition().x > _settings->getBattleFieldRect().size.width - (_settings->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 2 &&
                   canMove(unitAI, allUnits))
                {
                    useful = -1;
                }
                
                if(useful >= 0)
                {
                    if(useful == 0)
                    {                        
                        unitAI->setState(attM->animName);
                        //
                        
                            if(attM->type == AT_CALL && attM->remainedActionColdDown == 0 && attM->remainedColdDown == 0)
                            {
                                for (std::vector<CSummoned>::iterator iter = attM->vSummoned.begin(); iter < attM->vSummoned.end(); iter++)
                                {
                                    int randomSumm = rand()%((*iter).type.size());
                                    
                                    if ((*iter).type[randomSumm] == "mob")
                                    {
                                        unitAI->setState(attM->animName);
                                        
                                        cmdSetState* setsta = cmdSetState::createTS(unitAI, attM->animName);
                                        _commands->addCommand(setsta);
                                        setsta->release();
                                        
                                        AnimatedUnitModel* pUnit = MobFactory::createTS(atoi((*iter).value[randomSumm].c_str()), unitAI->pBattleModel);
                                        pUnit->setInvoker(unitAI);
                                        pUnit->setSpecialID(atoi((*iter).value[randomSumm].c_str()));
                                        
                                        pUnit->setTeam(unitAI->getTeam());
                                        pUnit->setLookMode(unitAI->getLookMode());
                                        pUnit->setLine(unitAI->getLine());
                                        
                                        //set position on line
                                        pUnit->setPosition(unitAI->getPosition());
                                        
                                        //set move direction
                                        pUnit->setMoveTarget(unitAI->getMoveTarget());   //move to user landing zone
                                        
                                        //add unit AI
                                        pUnit->addBehavior(ElementaryUnitAI::createTS(pUnit->pBattleModel));
                                        
                                        //add to battle model
                                        cmdAddUnit* add = cmdAddUnit::createTS(pUnit->pBattleModel, pUnit, 0);
                                        pUnit->pBattleModel->addCommand(add);
                                        
                                        attM->remainedColdDown = attM->coldDown;
                                        attM->remainedActionColdDown = attM->actionColdDown;
                                        
                                        add->release();
                                        pUnit->release();
                                    }
                                    
                                    if ((*iter).type[randomSumm] == "spell")
                                    {
                                        unitAI->setState(attM->animName);
                                        
                                        cmdSetState* setsta = cmdSetState::createTS(unitAI, attM->animName);
                                        _commands->addCommand(setsta);
                                        setsta->release();
                                        
                                        Character* hero = unitAI->getTeam() == 1 ? unitAI->pBattleModel->getUserHero() : unitAI->pBattleModel->getOpponentHero();
                                        int ads = atoi((*iter).value[randomSumm].c_str());
                                        spellAttack* sa = unitAI->getSpellAttack(ads);
                                        
                                        AnimatedUnitModel* _spellModel = new AnimatedUnitModel();
                                        BulletUnitAI* bAI = BulletUnitAI::createTS(unitAI->pBattleModel);
                                        bAI->addUntouchableUnit(3);
                                        _spellModel->addBehavior(bAI);
                                        bAI->release();
                                        _spellModel->setMaxHp(100);
                                        _spellModel->setPosition(unitAI->getPosition());
                                        _spellModel->setType(AUM_TYPE_SPELL);
                                        _spellModel->pBattleModel = unitAI->pBattleModel;
                                        
                                        _spellModel->setMoveDirection(unitAI->getMoveDirection());
                                        
                                        _spellModel->setSkin(sa->skin);
                                        _spellModel->setOriginSkinPoint(CCPointZero);
                                        CC_ASSERT(sa);
                                        
                                        UnitAttack* att = new UnitAttack();
                                        att->damage = sa->damage;
                                        att->damageDelta = sa->damageDelta;
                                        att->manaCost = sa->manaCost;
                                        att->range = sa->range;
                                        att->animName = sa->animName;
                                        att->damageTime = sa->damageTime;
                                        att->_bufs = sa->_bufs;
                                        att->animName = sa->animName;
                                        att->schoolName = sa->schoolName;
                                        
                                        
                                        
                                        _spellModel->addAttack(att);
                                        _spellModel->setSpecialID(atoi((*iter).value[randomSumm].c_str()));
                                        
                                        _spellModel->lifeTimeStarted = time(NULL) + 8; //Fire wall duration
                                        
                                        
                                        _spellModel->setMovingSpeed(0);
                                        _spellModel->setTeam(unitAI->getTeam());
                                        
                                        cmdAddUnit* add = cmdAddUnit::createTS(unitAI->pBattleModel, _spellModel, 0);
                                        unitAI->pBattleModel->addCommand(add);
                                        hero->setCastCmdID(add->getID());
                                        
                                        attM->remainedColdDown = attM->coldDown;
                                        attM->remainedActionColdDown = attM->actionColdDown;
                                        
                                        add->release();
                                    }
                                }
                            }

                        
                       
                        
                        cmdSetState* setsta = cmdSetState::createTS(unitAI, attM->animName);
                        _commands->addCommand(setsta);
                        setsta->release();
                    
                    
                        if(unitAI->getSkin() == "Orc_Dubolom" && unitAI->getCurAttackTarget()->getSkin() != "Orc_Dubolom" && unitAI->getCurAttackTarget()->getSkin() != "Frostling_Warrior" && unitAI->getCurAttackTarget()->getSpecialID() == 45 && unitAI->getType() == AUM_TYPE_MOB)
                        {
                            unitAI->getCurAttackTarget()->remainingCountPush = 30;
                        }
                    
                        //if action is that attack
                        if(unitAI->getAttack(priority)->type == AT_ENEMY)
                        {
                            //init cooldown decrements
                            attM->remainedColdDown = attM->coldDown;
                            attM->remainedActionColdDown = attM->actionColdDown;
                        
                        
                            //calculate critical attack
                            if(attM->criticalChance && attM->criticalChance > 0)
                            {
                                if(attM->critical)
                                {
                                    attM->critical = false;
                                    attM->damage /= attM->criticalPower;
                                }
                            
                                if((rand()%100 + 1) <= attM->criticalChance)
                                {
                                    attM->damage *= attM->criticalPower;
                                    attM->critical = true;
                                }
                            }
            
                        
                            if(attM->generatedUnit.length() == 0)      //melee unit action
                            {
                                if(unitAI->getSkin() == "Mehanic_Guard" || unitAI->getSkin() == "Demon_Rig")
                                {
                                    attM->remainingCountPush = unitAI->getAttack(priority)->countPush;
                                }
                                cmdAttack* cmdatt = cmdAttack::createTS(unitAI, unitAI->getCurAttackTarget(), attM);
                                _commands->addCommand(cmdatt);
                            }
                            else                                        //range unit action
                            {
                                BulletUnitAI* bAI = BulletUnitAI::createTS(_commands);
                                
                                //create BULLET unit
                                AnimatedUnitModel* unit = AnimatedUnitModel::createTSWithXML(attM->generatedUnit.c_str(), this->_commands);
                                
                                unit->setInvoker(unitAI);
                                unit->setSpecialID(priority);
                                
                                /*CCArray *arr = unitAI->getAttack(priority)->_bufs;
                                CCObject* obj;
                                CCARRAY_FOREACH(arr, obj)
                                {
                                    Buf *buf = Buf(obj);
                                    if(buf)
                                    {
                                        unit->getAttack()->_bufs->addObject(buf);
                                    }
                                }*/
                                unit->getAttack()->_bufs = unitAI->getAttack(priority)->_bufs;
                                
                                unit->setTeam(unitAI->getTeam());
                                
                                unit->setLine(unitAI->getLine());
                                
                                unit->setPosition(unitAI->getPosition());
                            
                                CCPoint dir = ccpNormalize(ccpSub(unitAI->getCurAttackTarget()->getPosition(), unitAI->getPosition()));
                                unit->setMoveDirection(dir);
                            
                                if(unitAI->getTeam() == 1)
                                    unit->setLookMode(AUM_LOOK_LEFT_MODE);
                                else
                                    unit->setLookMode(AUM_LOOK_RIGHT_MODE);
                            
                                //set config from MOB unit
                                unit->getAttack()->critical = unitAI->getAttack(priority)->critical;
                                unit->getAttack()->criticalChance = unitAI->getAttack(priority)->criticalChance;
                                unit->getAttack()->criticalPower = unitAI->getAttack(priority)->criticalPower;
                                unit->getAttack()->damage = unitAI->getAttack(priority)->damage;
                                unit->getAttack()->damageDelta = unitAI->getAttack(priority)->damageDelta;
                                unit->getAttack()->damageTime = unitAI->getAttack(priority)->damageTime;
                                unit->getAttack()->coldDown = unitAI->getAttack(priority)->coldDown;
                                unit->getAttack()->actionColdDown = unitAI->getAttack(priority)->actionColdDown;
                                unit->getAttack()->schoolName = unitAI->getAttack(priority)->schoolName;
                                
                                
                                if(unitAI->getSkin() == "Troll_Hunter" && rand()%100 + 1 > 50)
                                {
                                    unit->getAttack()->timeStun = unitAI->getAttack(priority)->timeStun;
                                }
                                
                                if(unitAI->getSkin() == "Frostling_Hunter" && rand()%100 + 1 > 50)
                                {
                                    unit->getAttack()->remainingCountPush = unitAI->getAttack(priority)->countPush;
                                }
                                unit->addBehavior(bAI);
                                bAI->release();
                                
                                cmdAddUnit* add = cmdAddUnit::createTS((BattleModel*)_commands, unit, attM->damageTime);
                                _commands->addCommand(add);
                            
                                add->release();
                                unit->release();
                            }
                        }
                    
                        //if action is that friendly skill
                        if(unitAI->getAttack(priority)->type == AT_FRIENDLY)
                        {
                            //for healer
                            assert(unitAI->getAttack(priority)->subtype == "heal" || unitAI->getAttack(priority)->subtype == "regeneration" || unitAI->getAttack(priority)->subtype == "raiseSkeletons"|| unitAI->getAttack(priority)->subtype == "resurrection");
                            if(unitAI->getAttack(priority)->subtype == "heal" || unitAI->getAttack(priority)->subtype == "regeneration")
                            {
                                //init cooldown decrements
                                attM->remainedColdDown = attM->coldDown;
                                attM->remainedActionColdDown = attM->actionColdDown;
                                cmdAttack* cmdatt = cmdAttack::createTS(unitAI, unitAI->getCurAttackTarget(), attM);
                                _commands->addCommand(cmdatt);
                            }else if(unitAI->getAttack(priority)->subtype == "resurrection")
                            {
                                attM->remainedColdDown = attM->coldDown;
                                attM->remainedActionColdDown = attM->actionColdDown;
                                unitAI->setActionWaitTime(attM->actionColdDown);
                                
                                unitAI->getCurAttackTarget()->setHp(unitAI->getMaxHp());
                                
                                unitAI->getCurAttackTarget()->setState("move1");
                                setsta = cmdSetState::createTS(unitAI->getCurAttackTarget(), "move1");
                                unitAI->pBattleModel->addCommand(setsta);
                                setsta->release();
                                
                                unitAI->setState(attM->animName);
                                
                                setsta = cmdSetState::createTS(unitAI, attM->animName);
                                _commands->addCommand(setsta);
                                setsta->release();
                                
                                /*
                                
                                unitAI->setState(attM->animName);
                                
                                cmdSetState* setsta = cmdSetState::createTS(unitAI, attM->animName);
                                _commands->addCommand(setsta);
                                setsta->release();
                                
                                AnimatedUnitModel* pUnit = MobFactory::createTS(unitAI->getCurAttackTarget()->getSpecialID(), unitAI->pBattleModel);
                                pUnit->setInvoker(unitAI);
                                pUnit->setSpecialID(unitAI->getCurAttackTarget()->getSpecialID());
                                
                                pUnit->setType(AUM_TYPE_MOB);
                                
                                pUnit->setTeam(unitAI->getTeam());
                                pUnit->setLookMode(unitAI->getLookMode());
                                pUnit->setLine(unitAI->getLine());
                                
                                //set position on line
                                pUnit->setPosition(unitAI->getCurAttackTarget()->getPosition());
                                
                                //set move direction
                                pUnit->setMoveTarget(unitAI->getMoveTarget());   //move to user landing zone
                                
                                //add unit AI
                                pUnit->addBehavior(ElementaryUnitAI::createTS(pUnit->pBattleModel));
                                
                                //add to battle model
                                cmdAddUnit* add = cmdAddUnit::createTS(pUnit->pBattleModel, pUnit, 0);
                                pUnit->pBattleModel->addCommand(add);
                                
                                attM->remainedColdDown = attM->coldDown;
                                attM->remainedActionColdDown = attM->actionColdDown;
                                
                                add->release();
                                pUnit->release();
                                
                                unitAI->pBattleModel->removeUnit(unitAI->getCurAttackTarget(), true);*/
                            }
                            else if(unitAI->getAttack(priority)->subtype == "raiseSkeletons")
                            {
                                unitAI->setState(attM->animName);
                                
                                cmdSetState* setsta = cmdSetState::createTS(unitAI, attM->animName);
                                _commands->addCommand(setsta);
                                setsta->release();
                                int idNewMob = 1;
                                switch (unitAI->getCurAttackTarget()->getTypeMob()) {
                                    case 1:
                                        idNewMob = 11;
                                        break;
                                    case 2:
                                        idNewMob = 10;
                                        break;
                                    case 3:
                                        idNewMob = 12;
                                        break;
                                    case 4:
                                        idNewMob = 9;
                                        break;
                                    default:
                                        break;
                                }
                                
                                AnimatedUnitModel* pUnit = MobFactory::createTS(idNewMob, unitAI->pBattleModel);
                                pUnit->setInvoker(unitAI);
                                pUnit->setSpecialID(idNewMob);
                                
                                pUnit->setType(AUM_TYPE_MOB);
                                
                                pUnit->setTeam(unitAI->getTeam());
                                pUnit->setLookMode(unitAI->getLookMode());
                                pUnit->setLine(unitAI->getLine());
                                
                                //set position on line
                                pUnit->setPosition(unitAI->getCurAttackTarget()->getPosition());
                                
                                //set move direction
                                pUnit->setMoveTarget(unitAI->getMoveTarget());   //move to user landing zone
                                
                                //add unit AI
                                pUnit->addBehavior(ElementaryUnitAI::createTS(pUnit->pBattleModel));
                                
                                //add to battle model
                                cmdAddUnit* add = cmdAddUnit::createTS(pUnit->pBattleModel, pUnit, 0);
                                pUnit->pBattleModel->addCommand(add);
                                
                                attM->remainedColdDown = attM->coldDown;
                                attM->remainedActionColdDown = attM->actionColdDown;
                                
                                add->release();
                                pUnit->release();
                                
                                unitAI->pBattleModel->removeUnit(unitAI->getCurAttackTarget(), true);
                            }
                        }
                    }
                    else
                    {
                        if(attM->type == AT_CALL && attM->remainedActionColdDown == 0)
                        {
                            if(canMove(unitAI, allUnits) && unitAI->getMovingSpeed() != 0)
                            {
                                unitAI->setState("move1");
                                
                                float part =  unitAI->getMovingSpeed() * dt / ccpDistance(unitAI->getMoveTarget(), unitAI->getPosition());
                                CCPoint resV = ccpMult(ccpSub(unitAI->getMoveTarget(), unitAI->getPosition()), part);
                                
                                unitAI->setPosition(ccpAdd(unitAI->getPosition(), resV));
                                
                                //set access to landing zone
                                landingZoneAccess(unitAI, allUnits);
                                
                                
                                // wait on center battle zone for singleplayer
                                if(_settings->getGameMode() == GM_SINGLEPLAYER &&
                                   unitAI->getTeam() == 1 &&
                                   unitAI->getPosition().x > _settings->getBattleFieldRect().size.width / 2)
                                {
                                    unitAI->setMovingSpeed(0);
                                    unitAI->setState("wait1");
                                }
                            }
                            else
                                unitAI->setState("wait1");
                        }else
                            unitAI->setActionWaitTime(useful);
                    }
                }
                else
                {
                    int* nAttack = new int();
                    sscanf(unitAI->getState().c_str(), "damage%d", nAttack);
                    if(*nAttack == 0)
                    {
                        if(canMove(unitAI, allUnits) && unitAI->getMovingSpeed() != 0)
                        {
                            unitAI->setState("move1");
            
                            float part =  unitAI->getMovingSpeed() * dt / ccpDistance(unitAI->getMoveTarget(), unitAI->getPosition());
                            CCPoint resV = ccpMult(ccpSub(unitAI->getMoveTarget(), unitAI->getPosition()), part);
                
                            unitAI->setPosition(ccpAdd(unitAI->getPosition(), resV));
                    
                            //set access to landing zone
                            landingZoneAccess(unitAI, allUnits);
                    
                    
                            // wait on center battle zone for singleplayer
                            if(_settings->getGameMode() == GM_SINGLEPLAYER &&
                               unitAI->getTeam() == 1 &&
                               unitAI->getPosition().x > _settings->getBattleFieldRect().size.width / 2)
                            {
                                unitAI->setMovingSpeed(0);
                                unitAI->setState("wait1");
                            }
                        }
                        else
                        {
                            unitAI->setState("wait1");
                        }
                    }
                    else
                    {
                        unitAI->setActionWaitTime(1);
                    }
                }
            }
        }
    }
    else
    {
        countPush = 0;
        unitAI->setState("death1");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void ElementaryUnitAI::landingZoneAccess(AnimatedUnitModel* unitAI, const cocos2d::CCArray *allUnits)
{
    float landingColsWidth = (_settings->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 2;
    
    
    //allow access for every line
    for(int i = 0; i < 3; i++)
        HUDScreen::getInstance()->landingZoneAccess[i] = true; 
    
    //deny access for appointed line
    CCObject* obj;
    CCARRAY_FOREACH(allUnits, obj)
    {
        AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
        
        //deny access to user landing zone
        if (unit->isAlive() &&
            unit->getType() == AUM_TYPE_MOB &&
            unit->getTeam() == 2 &&
            unit->getPosition().x < landingColsWidth + 20)
        {
            HUDScreen::getInstance()->landingZoneAccess[unit->getLine() - 1] = false;
        }
        
        //deny access to opponent hero AI landing zone
        if(_settings->getGameMode() == GM_SINGLE_AI)
        {
            if (unit->isAlive() &&
                unit->getType() == AUM_TYPE_MOB &&
                unit->getTeam() == 1 &&
                unit->getPosition().x > _settings->getBattleFieldRect().size.width - landingColsWidth - 20)
            {
                _commands->getOpponentHero()->pAI->landingZoneAccess[unit->getLine() - 1] = false;
            }
        }
    }
   

    if(unitAI->getTeam() == 2)
    {            
        if(unitAI->getPosition().x < landingColsWidth / 2)
        {
            
            
            //cause damage to the user hero
            _commands->getUserHero()->causeDamage(unitAI->getAttack(), unitAI->getAttack()->damage);
                
            //hide and remove unit
            unitAI->setPosition(ccp(512, -1000));
            cmdRemoveUnit* cmdrm = cmdRemoveUnit::createTS(_commands, unitAI, 0);
            _commands->addCommand(cmdrm);
            
            //allow access for this unit line
            HUDScreen::getInstance()->landingZoneAccess[unitAI->getLine() - 1] = true; 
        }
    }

    if(_commands->getOpponentHero() && unitAI->getTeam() == 1)
    {
        if(unitAI->getPosition().x > _settings->getBattleFieldRect().size.width - landingColsWidth / 2)
        {
            //cause damage to the opponent hero
            _commands->getOpponentHero()->causeDamage(unitAI->getAttack(), unitAI->getAttack()->damage);
                
            //hide and remove unit
            unitAI->setPosition(ccp(512, -1000));
            cmdRemoveUnit* cmdrm = cmdRemoveUnit::createTS(_commands, unitAI, 0);
            _commands->addCommand(cmdrm);
            
            //allow access for this unit line
            if(_settings->getGameMode() == GM_SINGLE_AI)
                _commands->getOpponentHero()->pAI->landingZoneAccess[unitAI->getLine() - 1] = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
CCPoint ElementaryUnitAI::pathFind(const AnimatedUnitModel* unitAI,const AnimatedUnitModel* unit)
{
    CCPoint resV = ccpSub(unit->getPosition(),unitAI->getPosition());
    
    
    Settings* set = Settings::sharedInstance();
    int step = set->getBattleFieldRect().size.width / set->getMobAIMaxDirectDistance();
    float delta = set->getBattleFieldRect().size.height / set->getMobAIFakeTargetVariance();
    if (ccpLength(resV)<step)
        return ccpAdd(unitAI->getPosition(), resV);
    else
    {
        float k = (float) step / ccpLength(resV);
        CCPoint fakevector = ccpAdd(ccpMult(resV, k),ccp(0,CCRANDOM_MINUS1_1()*delta));
        CCPoint faketarget = ccpAdd(unitAI->getPosition(), fakevector);
        
        // fake target could be out of the BattleField
        faketarget.y = MAX(faketarget.y,0);
        faketarget.y = MIN(faketarget.y,set->getBattleFieldRect().size.height);
        
        return faketarget;
    }
}