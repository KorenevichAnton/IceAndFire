//
//  ChoiceSpellsScene.h
//  BattleMagic
//
//  Created by madmin on 1/2/14.
//
//

#ifndef __BattleMagic__ChoiceSpellsScene__
#define __BattleMagic__ChoiceSpellsScene__


#include "cocos2d.h"
#include "Settings.h"
#include <vector.h>
#include "SpellCard.h"


//---------------------------------------------------------------------------------------------------------------------------------
class ChoiceSpells : public cocos2d::CCLayer
{
public:
    
    static ChoiceSpells* create();
    void selectSpellFromDeck();
    ~ChoiceSpells();
    
private:
    ChoiceSpells();
    bool                isEndScene;
    
    bool    init();
    
    float screenWidth;
    float screenHeight;
    
    //touch events
    void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    
    //spellCards
    
    SpellCard*            deck[16];
    SpellCard*            selectedSpellCards[4];
    
    //
    cocos2d::CCSprite*    selectedSpellIcons[4];
    //
    cocos2d::CCMenu*            pMenuForButton;
    cocos2d::CCMenuItemSprite*  btnApply;
    cocos2d::CCMenuItemSprite*  btnBack;
    cocos2d::CCMenuItemSprite*  btnWaterSpells;
    cocos2d::CCMenuItemSprite*  btnFireSpells;
    cocos2d::CCMenuItemSprite*  btnEarthSpells;
    cocos2d::CCMenuItemSprite*  btnAirSpells;
    
    //Elements Icons
    cocos2d::CCSprite*    iconElement;
    //Spells Icons
    cocos2d::CCSprite*    iconSpellsOrder[4];
    cocos2d::CCSprite*    iconSpellsSelect[4];
    cocos2d::CCSprite*    iconSpellsLock[4];
    //
    cocos2d::CCSprite*    waterDekor;
    cocos2d::CCSprite*    fireDekor;
    cocos2d::CCSprite*    earthDekor;
    cocos2d::CCSprite*    airDekor;
    //
    
    //switching elements
    void showSpells(int beginID, int endID);
    
    void onBtnApply(CCObject* pSender);
    void onBtnBack(CCObject* pSender);
    
    void onBtnWaterSpells(CCObject* pSender);
    void onBtnFireSpells(CCObject* pSender);
    void onBtnEarthSpells(CCObject* pSender);
    void onBtnAirSpells(CCObject* pSender);
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__ChoiceSpellsScene__) */
