#include "BattleModel.h"
#include "Settings.h"
#include "CCProgressBar.h"
#include "xmlParser.h"
#include "Utils.h"
#include "Network.h"
#include "LevelModel.h"
#include "NetworkControllerWrap.h"

USING_NS_CC;

//---------------------------------------------------------------------------------------------------------------------------------
BattleModel::BattleModel():
_units(NULL),
_user_hero(NULL),
_opponent_hero(NULL),
_listener(NULL),
_lvlModel(NULL)
{
    _commands = CCArray::create();
    CC_SAFE_RETAIN(_commands);
    
    _units = CCArray::create();
    CC_SAFE_RETAIN(_units);
}

//---------------------------------------------------------------------------------------------------------------------------------
BattleModel::~BattleModel()
{
    CC_SAFE_RELEASE_NULL(_commands);
    CC_SAFE_RELEASE_NULL(_units);
    CC_SAFE_RELEASE(_lvlModel);
    
    AUStateListenableInterfeace::resetCounter();
}
//---------------------------------------------------------------------------------------------------------------------------------
BattleModel* BattleModel::createWithGlobalMap()
{
    BattleModel* pRet = new BattleModel();
    
    pRet->scheduleUpdate();
    
    cmdHeroInit* cmdInit = cmdHeroInit::createTS(pRet);
    if(cmdInit)
    {
        pRet->addCommand(cmdInit);
        cmdInit->release();
    }
    
    pRet->_lvlModel = LevelModel::createTSWithGlobalMap(pRet);
    //CC_SAFE_RETAIN(pRet->_lvlModel);

    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
BattleModel* BattleModel::createWithXML(const std::string &xmlFileName)
{
    BattleModel* pRet = new BattleModel();
    if(pRet && pRet->initWithXML(xmlFileName))
        return pRet;
    else
    {
        delete pRet;
        return NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool BattleModel::initWithXML(const std::string &xmlFileName)
{
    this->scheduleUpdate();
    
    cmdHeroInit* cmdInit = cmdHeroInit::createTS(this);
    if(cmdInit)
    {
        addCommand(cmdInit);
        cmdInit->release();
    }
    
    _lvlModel = LevelModel::createTS(this, xmlFileName);
    //CC_SAFE_RETAIN(_lvlModel);
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
Character* BattleModel::getUserHero() const
{
    return _user_hero;
}

//---------------------------------------------------------------------------------------------------------------------------------
Character* BattleModel::getOpponentHero() const
{
    return _opponent_hero;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleModel::setUserHero(Character* _hero)
{
    if(!_user_hero)
        _user_hero = _hero;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleModel::setOpponentHero(Character* _hero)
{
    if(!_opponent_hero)
        _opponent_hero = _hero;
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleModel::addUnit(AnimatedUnitModel* unit, bool inform)
{
    _units->addObject(unit);
    if (inform && _listener)
        _listener->addUnit(unit);
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleModel::removeUnit(AnimatedUnitModel* unit, bool inform)
{
    if(unit)
    {
        _units->removeObject(unit);
        
        if(inform && _listener)
            _listener->removeUnit(unit);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void BattleModel::showMessage(AnimatedUnitModel* _unit, const char* _msg, const cocos2d::ccColor3B _color, cocos2d::CCPoint _offset)
{
    if(_listener)
       _listener->showMessage(_unit, _msg, _color, _offset);
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void BattleModel::update(float dt)
{
    if(Settings::sharedInstance()->getGameMode() != GM_MULTIPLAYER)
    {
        CCObject* obj;
        CCArray* forremove = NULL;
    
        if(Settings::sharedInstance()->getGameMode() == GM_SINGLEPLAYER)
        {
            if (_lvlModel)
                _lvlModel->update(dt);
        }
    
        CCARRAY_FOREACH(_units, obj)
        {
            AnimatedUnitModel* unit = (AnimatedUnitModel*) obj;
            unit->update(_units, dt);
        }
        
        
        forremove = NULL;
        
        CCARRAY_FOREACH(_commands, obj)
        {
            BattleCommand* cmd = (BattleCommand*)obj;
            if(cmd->_remainedTime > 0)
                cmd->_remainedTime -= dt;
            else
            {
                if(!forremove)
                    forremove = CCArray::create();
                forremove->addObject(obj);
            }
        }
    
        if(forremove)
            _commands->removeObjectsInArray(forremove);
    
        CCARRAY_FOREACH(forremove, obj)
        {
            BattleCommand* cmd = (BattleCommand*)obj;
            cmd->execute();
            this->removeCommand(cmd);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleCommandQueue::addCommand(BattleCommand* cmd)
{
    if(cmd)
        _commands->addObject(cmd);
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleCommandQueue::removeCommand(BattleCommand* cmd)
{
    if(cmd)
        _commands->removeObject(cmd);
}

//---------------------------------------------------------------------------------------------------------------------------------
void BattleCommandQueue::removeCommand(int id)
{
    BattleCommand* res = NULL;
    
    CCObject* obj;
    CCARRAY_FOREACH(_commands, obj)
    {
        if (((BattleCommand*)obj)->getID() == id)
            res= (BattleCommand*) obj;
    }
    
    removeCommand(res);
}

//---------------------------------------------------------------------------------------------------------------------------------