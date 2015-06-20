#include "EarthHolySpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdAttack.h"
#include "bufAll.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
EarthHolySpellAI::EarthHolySpellAI():
UnitAI(),
typeUnitAI(EARTH_HOLY_SPELL_AI),
initialize(false),
threshold(0),
position(ccp(0, 0)),
firstCPPosition(ccp(0, 0)),
secondCPPosition(ccp(0, 0))
{
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthHolySpellAI::~EarthHolySpellAI()
{
    bufedUnits.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthHolySpellAI* EarthHolySpellAI::createTS(BattleModel* _battleModel)
{
    EarthHolySpellAI* pRet = new EarthHolySpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthHolySpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /* here a initialize default parameters
    */
    
    initCastingArea();
    
    //indentation from the edge of the screen
    float offset = (Settings::sharedInstance()->getBattleFieldRect().size.width - BATTLE_ZONE_WIDTH) / 4;

    //set spell position
    _unitAI->setPosition(cocos2d::CCPoint( (_unitAI->getTeam() == 1) ? (offset) : (Settings::sharedInstance()->getBattleFieldRect().size.width - offset),
                                          _unitAI->getPosition().y ));

    threshold = 0.5;


    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthHolySpellAI::setPosition(cocos2d::CCPoint _position)
{
    position = _position;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthHolySpellAI::initCastingArea()
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
void EarthHolySpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{   
    if(!initialize)
        this->init(_unitAI, _allUnits);
    
    if(_unitAI->isAlive())
    {
        CCObject* obj = NULL;
        CCARRAY_FOREACH(_allUnits, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            if( unit->isMob() &&
                unit->getTeam() == _unitAI->getTeam() &&
                unit->getPosition().x > firstCPPosition.x && unit->getPosition().x < secondCPPosition.x)
                //unit->getLine() == _unitAI->getLine() )
            {
                //send bufs from attack to target unit
                CCObject* objBuf;
                CCARRAY_FOREACH(_unitAI->getAttack()->_bufs, objBuf)
                {
                    Buf* buf = (Buf*)objBuf;
                    if(buf->getKind() == BUF_SEND)
                    {
                        if(buf->getType() == BUF_REGENERATION)
                        {
                            if(unit->isAlive())
                            {
                                buf->setTime( _unitAI->getLifeTime() );
                                unit->addBuf( buf );
                            }
                        }
                        
                        if(buf->getType() == BUF_RESURRECTION)
                        {
                            if(!unit->isAlive())
                            {
                                buf->setTime( _unitAI->getLifeTime() );
                                unit->addBuf( buf );
                            }
                        }
                    }
                }
                //
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool EarthHolySpellAI::isBufed(AnimatedUnitModel* _unit)
{
    for(vector<AnimatedUnitModel*>::const_iterator i = bufedUnits.begin(); i < bufedUnits.end(); i++)
    {
        if((*i) == _unit)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
