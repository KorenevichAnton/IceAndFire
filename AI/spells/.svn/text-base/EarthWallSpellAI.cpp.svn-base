#include "EarthWallSpellAI.h"
#include "Settings.h"
#include "BattleModel.h"
#include "cmdRemoveUnit.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
EarthWallSpellAI::EarthWallSpellAI():
UnitAI(),
typeUnitAI(EARTH_WALL_SPELL_AI),
initialize(false)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthWallSpellAI::~EarthWallSpellAI()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
EarthWallSpellAI* EarthWallSpellAI::createTS(BattleModel* _battleModel)
{
    EarthWallSpellAI* pRet = new EarthWallSpellAI();
    if(pRet)
        pRet->pBattleModel = _battleModel;
    
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthWallSpellAI::init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits)
{
    /*
     here a set spell position and initialize default parameters
    */
    
    if(_unitAI->isAlive())
    {
        int bfHeight = Settings::sharedInstance()->getBattleFieldRect().size.height;
        int offset = 50;
        
        //change the spell on mob
        _unitAI->setType(AUM_TYPE_MOB);
        _unitAI->setMaxHp(1000);
        _unitAI->setHp(_unitAI->getMaxHp());
        

        //for normal positioning as target
        if(_unitAI->getTeam() == 1)
            position.x += offset + 20;
        else
            position.x -= offset;
        _unitAI->setPosition( ccp(position.x, 0) );
        
        
        //set physic model
        PhysicUnitModel* physic = new PhysicUnitModel();
        
        //set body for lock mobs movement
        physic->setCircleOrigin( ccp(0, bfHeight / 2) );
        physic->setRadius(bfHeight);
        
        //set body for receive attack from mobs
        physic->addToPolygon( cocos2d::CCPoint(-35, 0) );
        physic->addToPolygon( cocos2d::CCPoint(-35, bfHeight) );
        physic->addToPolygon( cocos2d::CCPoint(35, bfHeight) );
        physic->addToPolygon( cocos2d::CCPoint(35, 0) );
        
        _unitAI->setPhysicalModel(physic);
    }
    
    
    initialize = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthWallSpellAI::setPosition(cocos2d::CCPoint _position)
{
    position = _position;
}

//---------------------------------------------------------------------------------------------------------------------------------
cocos2d::CCPoint EarthWallSpellAI::getPosition()
{
    return position;
}

//---------------------------------------------------------------------------------------------------------------------------------
void EarthWallSpellAI::update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt)
{
    if(!initialize)
        this->init(_unitAI, _allUnits);
    
    if(_unitAI->isAlive())
    {
    }
    else
    {
        cmdRemoveUnit* rem = cmdRemoveUnit::createTS(pBattleModel, _unitAI, 0);
        pBattleModel->addCommand(rem);
        rem->release();
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
