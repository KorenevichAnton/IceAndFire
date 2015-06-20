//
//  SpellCard.h
//  BattleMagic
//
//  Created by madmin on 1/3/14.
//
//

#ifndef __BattleMagic__SpellCard__
#define __BattleMagic__SpellCard__

#include "cocos2d.h"
#include "BattleModel.h"
#include "Clipping.h"
#include <vector>
#include "ICard.h"

class ChoiceSpells;

class SpellCard : public ICard
{
public:
    
    static SpellCard*           initWithXML(const std::string& _xmlFileName, int spellNumber);
    static void                 destroy();
    
    cocos2d::CCSprite*          getIcon() const;
    cocos2d::CCSprite*          getBigIcon() const;
    cocos2d::CCSprite*          getSelectIcon() const;
    cocos2d::CCMenuItemImage*   getBtnBorder() const;
    cocos2d::CCSprite*          getSelectBorder() const;
    cocos2d::CCSprite*          getLockBorder() const;
    cocos2d::CCLayer*           getSpellDescription() const;
    cocos2d::CCSprite*          getManaCostIcon() const;
    cocos2d::CCSprite*          getAttackIcon() const;
    cocos2d::CCSprite*          getCloserPanelPayGold() const;
    cocos2d::CCSprite*          getCloserPanelPayCrystall() const;
    void                        onBtnPayGold(cocos2d::CCObject* pSender);
    void                        onBtnPayCrystall(cocos2d::CCObject* pSender);
    void                        onBtnSelect(cocos2d::CCObject* pSender);
    
    void                        selectSpellFromDeck();
    
    bool getSpellDisplayVisible() const;
    void setSpellDisplayVisible(const bool _state);  //show/hide display of spell information
    
    void setAvailable(const bool _state);
    
    bool getSelected() const;
    void setSelected(const bool _state);
    
    void setLockState(const bool _state);
    
    int                 getSpellID() const;
    int                 getSpellCardID() const;
    std::string         getName() const;
    std::string         getDescription() const;
    float               getDamage() const;
    float               getManaCost() const;
    std::string         getSchool() const;
    int                 getRequirements() const;
    
private:
    static std::vector<SpellCard*> spellCardDeck;
    
    
    int                 spellCardID;
    int                 spellID;
    std::string         name;
    std::string         description;
    float               damage;
    float               manaCost;
    std::string         school;
    int                 requirements;    
    
    
    cocos2d::CCMenu*            pMenuButton;
    cocos2d::CCSprite*          pIcon;
    cocos2d::CCSprite*          pManaCostIcon;
    cocos2d::CCSprite*          pAttackIcon;
    cocos2d::CCSprite*          pBigIcon;
    cocos2d::CCSprite*          pSelectIcon;
    cocos2d::CCMenuItemImage*   pBorder;
    cocos2d::CCSprite*          pSelectBorder;
    cocos2d::CCSprite*          pLockBorder;

    
    //the window of detail information about a spell
    void createSpellDisplay();
    cocos2d::CCLayer*           pSpellDisplay;
    cocos2d::CCLayer*           pSpellDescription;
    
    cocos2d::CCSprite*          pCloserPanelPayGold;
    cocos2d::CCSprite*          pCloserPanelPayCrystall;
    
    cocos2d::CCLabelTTF*        pCloserHeader;
    SpellCard();
    ~SpellCard();
    
};


#endif /* defined(__BattleMagic__SpellCard__) */
