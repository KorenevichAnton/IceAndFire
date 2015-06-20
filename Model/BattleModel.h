#ifndef __BattleMagic__BattleModel__
#define __BattleMagic__BattleModel__

#include "cocos2d.h"
#include "AnimatedUnitModel.h"
#include "Character.h"
#include "cmdHeroInit.h"
#include "cmdAddUnit.h"
#include "BattleListenerInterface.h"

class cmdRemoveUnit;
class LevelModel;

//---------------------------------------------------------------------------------------------------------------------------------
class BattleCommandQueue
{
protected:
    cocos2d::CCArray*   _commands;  //Queue of orders to BattleModel
    
public:
    void addCommand(BattleCommand* cmd);
    void removeCommand(BattleCommand* cmd);
    void removeCommand(int id);
};

//---------------------------------------------------------------------------------------------------------------------------------
class BattleModel : public cocos2d::CCNode, public BattleCommandQueue
{

protected:
    cocos2d::CCArray*   _units;     // all unit on the field
    
    Character*  _user_hero;         // user character
    Character*  _opponent_hero;     // opponent character
    
    BattleListenerInterface*    _listener;  //not retain
    
    LevelModel*     _lvlModel;
    
public:
    BattleModel();
    virtual ~BattleModel();
    CREATE_FUNC(BattleModel);
    
    virtual bool    init(){ return true; }
    void            update(float dt);  // main loop
    
    void setListener(BattleListenerInterface* listner){ _listener=listner; }
    
    
    // for exectute all commands in this iteration not in the next
    void pushExecution() {update(0);}
    
    //get character
    Character* getUserHero() const;
    Character* getOpponentHero() const;
    
    //set character
    void setUserHero(Character* _hero);
    void setOpponentHero(Character* _hero);
    
    // add/remove Units methods with inform view flag
    void addUnit(AnimatedUnitModel* unit,bool inform);
    void removeUnit(AnimatedUnitModel* unit,bool inform);
    
    void showMessage(AnimatedUnitModel* _unit, const char* _msg, const cocos2d::ccColor3B _color, cocos2d::CCPoint _offset = cocos2d::CCPoint(0,0));
    
    
    const cocos2d::CCArray* getUnits() const { return _units;}
    
    bool                initWithXML(const std::string& xmlFileName);
    static BattleModel* createWithXML(const std::string& xmlFileName);
    static BattleModel* createWithGlobalMap();
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__BattleModel__) */
