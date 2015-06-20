#include "AirLightningSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"
#include "cmdRemoveUnit.h"
#include "bufAll.h"


using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
AirLightningSpellAI::AirLightningSpellAI():
UnitAI(),
typeUnitAI(AIR_LIGHTNING_SPELL_AI),
initialize(false)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
AirLightningSpellAI::~AirLightningSpellAI()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
AirLightningSpellAI* AirLightningSpellAI::createTS(BattleModel* _battleModel)
{
    AirLightningSpellAI* pRet = new AirLightningSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void AirLightningSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */

    //create splash attack
    UnitAttack* splashAttack = new UnitAttack();
    splashAttack->priority = 1;
    splashAttack->type = AT_ENEMY;
    splashAttack->schoolName = "air";
    splashAttack->range = 100;    //splash radius!
    splashAttack->damage = 0;
    splashAttack->damageDelta = 0;
    splashAttack->dmgColor = _unitAI->getAttack()->dmgColor;

    //add push buf from spell attack to splash attack
    CCObject* objBuf;
    CCARRAY_FOREACH(_unitAI->getAttack()->_bufs, objBuf)
    {
        Buf* buf = (Buf*)objBuf;
        if(buf->getType() == BUF_PUSH)
        {
            Buf* newcopy = buf->copy();
            splashAttack->_bufs->addObject(newcopy);
            newcopy->release();
        }
    }

    _unitAI->addAttack(splashAttack);
    
    splashAttack->release();
    //

    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void AirLightningSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);

    if(_unitAI->isAlive())
    {
        //move
        _unitAI->setPosition(ccpAdd(_unitAI->getPosition(), ccpMult(_unitAI->getMoveDirection(),_unitAI->getMovingSpeed()*_dt)));


        CCObject* obj = NULL;
        CCARRAY_FOREACH(_allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            int priority = 0;
            if( unit->isAlive() &&
                unit->isMob() &&
                unit->getTeam() != _unitAI->getTeam() &&
                _unitAI->getCurAttackTarget() == unit &&
                UnitAI::canAttack(_unitAI, unit, priority))
            {
                cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unit, _unitAI->getAttack(), 0);
                pBattleModel->addCommand(cmdAtt);
                cmdAtt->release();


                //splash attack
                CCObject* objSp = NULL;
                CCARRAY_FOREACH(_allUnits, objSp)
                {
                    AnimatedUnitModel* unitSp = (AnimatedUnitModel*) objSp;
                    int priority = 1;
                    if( unitSp != unit &&
                        unitSp->isAlive() &&
                        unitSp->isMob() &&
                        unitSp->getTeam() != _unitAI->getTeam() &&
                        unitSp->getLine() == _unitAI->getLine() &&
                        //check for membership in splash radius
                        unitSp->getPosition().x > unit->getPosition().x - _unitAI->getAttack(priority)->range &&
                        unitSp->getPosition().x < unit->getPosition().x + _unitAI->getAttack(priority)->range )
                    {
                        cmdAttack* cmdAtt = cmdAttack::createTS(pBattleModel, _unitAI, unitSp, _unitAI->getAttack(priority), 0);
                        pBattleModel->addCommand(cmdAtt);
                        cmdAtt->release();
                    }
                }
                //

                cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, 0.5);  //delay for play death animation
                pBattleModel->addCommand(rem);
                rem->release();

                _unitAI->setHp(0);  //to avoid repeating commands

                break;
            }
        }

        //border controll
        if (!Settings::sharedInstance()->getDeathRect().containsPoint(_unitAI->getPosition()))
        {
            cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, 0);
            pBattleModel->addCommand(rem);
            rem->release();

            _unitAI->setHp(0);  //to avoid repeating commands
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
