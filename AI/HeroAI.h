#ifndef __BattleMagic__HeroAI__
#define __BattleMagic__HeroAI__

#include "cocos2d.h"
#include "BattleModel.h"
#include "Card.h"

//---------------------------------------------------------------------------------------------------------------------------------
class HeroAI : public cocos2d::CCObject
{
public:
    static HeroAI*  createForCharacter(Character* _hero, BattleModel* _battleModel);
    void            update(float _dt);
    
    void createMob(int _mobID, int _line);
    void createSpell(int _spellID, int _line);
    
        
    
private:
    Character*      pCharacter;
    BattleModel*    pBattleModel;
    
    //cards
    Card*           cards[9];
    float           cardsCD;    //card cooldown
    
    //spells
    spellAttack*    spells[4];
    float           spellsCD;   //spell cooldown
    
    
    //dispersion mobs by Y on lines
    int             dispersionMobs[3];
        
    float           secondsForStart;
    
    HeroAI();
    ~HeroAI();
};

//---------------------------------------------------------------------------------------------------------------------------------
/** AI config */
#define COMMON_CARD_CD 10;
#define COMMON_SPELL_CD 15;

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__HeroAI__) */
