#ifndef __BattleMagic__SpellFactory__
#define __BattleMagic__SpellFactory__

#include "AnimatedUnitModel.h"

class spellAttack;
class BattleModel;

//---------------------------------------------------------------------------------------------------------------------------------
class SpellFactory:public cocos2d::CCObject
{
protected:
    BattleModel*   _commands;
    
    public :
    
    SpellFactory(){_commands = NULL;}
    virtual     ~SpellFactory();
    
    virtual     AnimatedUnitModel* createTSSpell(spellAttack* attack,
                                                 const cocos2d::CCPoint& targetPoint,
                                                 const cocos2d::CCPoint& casterPoint,
                                                 cocos2d::ccColor3B color,
                                                 int _unitID = 0) const = 0;
};

//---------------------------------------------------------------------------------------------------------------------------------
class PngSequenceSpellFactory:public SpellFactory
{
public:
    AnimatedUnitModel* createTSSpell(spellAttack* attack,
                                     const cocos2d::CCPoint& targetPoint,
                                     const cocos2d::CCPoint& casterPoint,
                                     cocos2d::ccColor3B color,
                                     int _unitID = 0) const;
    bool init(){ return true; }
    
    CREATE_FUNC(PngSequenceSpellFactory);
};

//---------------------------------------------------------------------------------------------------------------------------------
class ParticleSpellFactory:public SpellFactory
{
protected:
    ParticleSpellFactory();
    int  getTouchesLine(const cocos2d::CCPoint position) const;
    
public:
    AnimatedUnitModel* createTSSpell(spellAttack* attack,
                                     const cocos2d::CCPoint& targetPoint,
                                     const cocos2d::CCPoint& casterPoint,
                                     cocos2d::ccColor3B color,
                                     int _unitID = 0) const;
    
    static ParticleSpellFactory* createTS(BattleModel* commands);
    
    bool init(BattleModel*   commands);
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__SpellFactory__) */
