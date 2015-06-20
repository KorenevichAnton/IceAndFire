#include "EarthPowerSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"
#include "bufAll.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
EarthPowerSpellAI::EarthPowerSpellAI():
UnitAI(),
typeUnitAI(EARTH_POWER_SPELL_AI),
position(ccp(0, 0)),
firstCPPosition(ccp(0, 0)),
secondCPPosition(ccp(0, 0)),
initialize(false)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthPowerSpellAI::~EarthPowerSpellAI()
{
    bufedUnits.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthPowerSpellAI* EarthPowerSpellAI::createTS(BattleModel* _battleModel)
{
    EarthPowerSpellAI* pRet = new EarthPowerSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthPowerSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */
    initCastingArea();
    //indentation from the edge of the screen
    float offset = (Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 4;

    //set spell position
    _unitAI->setPosition(cocos2d::CCPoint( (_unitAI->getTeam() == 1) ? (offset) : (Settings::sharedInstance()->getBattleFieldRect().size.width - offset),
                                          _unitAI->getPosition().y ));


    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthPowerSpellAI::setPosition(cocos2d::CCPoint _position)
{
    position = _position;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthPowerSpellAI::initCastingArea()
{
    if(position.x > 160)
    {
        firstCPPosition.x = 160;
        secondCPPosition.x = 291;
    }
    if(position.x > 351)
    {
        firstCPPosition.x = 351;
        secondCPPosition.x = 482;
    }
    if(position.x > 542)
    {
        firstCPPosition.x = 542;
        secondCPPosition.x = 673;
    }
    if(position.x > 733)
    {
        firstCPPosition.x = 733;
        secondCPPosition.x = 864;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthPowerSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);

    if(_unitAI->isAlive())
    {
        CCObject* obj = NULL;
        CCARRAY_FOREACH(_allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            if( unit->isAlive() &&
                unit->isMob() &&
                unit->getTeam() == _unitAI->getTeam() &&
                //unit->getLine() == _unitAI->getLine() &&
                unit->getPosition().x > firstCPPosition.x && unit->getPosition().x < secondCPPosition.x &&
                !isBufed(unit) )
            {
                //send bufs from attack to target unit
                
                CCObject* objBuf;
                CCARRAY_FOREACH(_unitAI->getAttack()->_bufs, objBuf)
                {
                    Buf* buf = (Buf*)objBuf;
                    if(buf->getKind() == BUF_SEND)
                    {
                        buf->setTime(_unitAI->getLifeTime());
                        unit->addBuf(buf);
                    }
                }
                bufedUnits.push_back(unit);

            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool EarthPowerSpellAI::isBufed(AnimatedUnitModel* _unit)
{
    for(vector<AnimatedUnitModel*>::const_iterator i = bufedUnits.begin(); i < bufedUnits.end(); i++)
    {
        if((*i) == _unit)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
