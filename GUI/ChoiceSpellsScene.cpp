//
//  ChoiceSpellsScene.cpp
//  BattleMagic
//
//  Created by madmin on 1/2/14.
//
//

#include "ChoiceSpellsScene.h"

#include "NetworkControllerWrap.h"
#include "MemoryProfiler.h"
#include "MyBattleScene.h"
#include "ChoiceCardsScene.h"
#include <math.h>
#include "Timer.h"
#include "xmlParser.h"
#include "Utils.h"
#include "Settings.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
ChoiceSpells::ChoiceSpells():
screenWidth(0),
screenHeight(0),
//spell cards
pMenuForButton(NULL),
btnApply(NULL),
btnBack(NULL),
btnWaterSpells(NULL),
btnAirSpells(NULL),
btnEarthSpells(NULL),
btnFireSpells(NULL),
iconElement(NULL),
waterDekor(NULL),
fireDekor(NULL),
earthDekor(NULL),
airDekor(NULL)
{
    for(int i = 0 ; i < 4 ; i++)
        iconSpellsLock[i] = NULL;
    
    for(int i = 0 ; i < 4 ; i++)
        iconSpellsOrder[i] = NULL;
    
    for(int i = 0 ; i < 4 ; i++)
        iconSpellsSelect[i] = NULL;
    
    for(int i = 0 ; i < 4 ; i++)
        selectedSpellCards[i] = NULL;
    
    for(int i = 0 ; i < 16 ; i++)
        deck[i] = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
ChoiceSpells::~ChoiceSpells()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
ChoiceSpells* ChoiceSpells::create()
{
    ChoiceSpells* obj = new ChoiceSpells();
    if (obj && obj->init())
    {
        obj->autorelease();
        return obj;
    }
    else
    {
        delete obj;
        obj = NULL;
        return NULL;
    }
    return obj;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool ChoiceSpells::init()
{
    //super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    isEndScene = false;
    
    //ask screen size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    screenWidth = size.width;
    screenHeight = size.height;
    
    
    // add background image
    CCSprite* pSprite = CCSprite::create("Textures/GUI/ChoiceSpells/back_choicespells.png");
    pSprite->setPosition(ccp(screenWidth / 2, screenHeight / 2));
    this->addChild(pSprite, 0);
    
    
    //TEMP
    //loading atlas
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Textures/GUI/HUD/Hud_all_01.plist");
    
    CCSprite* bApply = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Button_Start_Fight.png");
    CCSprite* bBack = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Button_Back.png");
    
    CCSprite* bWaterSpellsPress = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Water_Sign_Press.png");
    CCSprite* bFireSpellsPress = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Fire_Sign_Press.png");
    CCSprite* bEarthSpellsPress = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Earth_Sign_Press.png");
    CCSprite* bAirSpellsPress = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Air_Sign_Press.png");
    
    CCSprite* bWaterSpells = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Water_Sign.png");
    CCSprite* bFireSpells = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Fire_Sign.png");
    CCSprite* bEarthSpells = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Earth_Sign.png");
    CCSprite* bAirSpells = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Tab_Air_Sign.png");
    
    CCSprite* bApplyPress = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Button_Press_Start_Fight.png");
    CCSprite* bBackPress = cocos2d::CCSprite::create("Textures/GUI/ChoiceSpells/Button_Press_Back.png");
    
    waterDekor = CCSprite::create("Textures/GUI/ChoiceSpells/Water_Dekor.png");
    waterDekor->setAnchorPoint(ccp(0, 0));
    waterDekor->setPosition(ccp(125, 250));
    waterDekor->setVisible(true);
    this->addChild(waterDekor);
    
    
    fireDekor = CCSprite::create("Textures/GUI/ChoiceSpells/Fire_Dekor.png");
    fireDekor->setAnchorPoint(ccp(0, 0));
    fireDekor->setPosition(ccp(125, 250));
    fireDekor->setVisible(false);
    this->addChild(fireDekor);
    
    
    earthDekor = CCSprite::create("Textures/GUI/ChoiceSpells/Earth_Dekor.png");
    earthDekor->setAnchorPoint(ccp(0, 0));
    earthDekor->setPosition(ccp(125, 250));
    earthDekor->setVisible(false);
    this->addChild(earthDekor);
    
    airDekor = CCSprite::create("Textures/GUI/ChoiceSpells/Air_Dekor.png");
    airDekor->setAnchorPoint(ccp(0, 0));
    airDekor->setPosition(ccp(125, 250));
    airDekor->setVisible(false);
    this->addChild(airDekor);
    
    btnApply = CCMenuItemSprite::create(bApply, bApplyPress, this, menu_selector(ChoiceSpells::onBtnApply));
    btnApply->setAnchorPoint(ccp(0, 0));
    btnApply->setPosition(ccp(838, 88));
    
    btnBack = CCMenuItemSprite::create(bBack, bBackPress, this, menu_selector(ChoiceSpells::onBtnBack));
    btnBack->setAnchorPoint(ccp(0, 0));
    btnBack->setPosition(ccp(33, 88));
    //Switch between spells
    
    btnWaterSpells = CCMenuItemSprite::create(bWaterSpells, bWaterSpellsPress, this, menu_selector(ChoiceSpells::onBtnWaterSpells));
    btnWaterSpells->setAnchorPoint(ccp(0, 0));
    btnWaterSpells->setPosition(ccp(557, 625));
    
    btnFireSpells = CCMenuItemSprite::create(bFireSpells, bFireSpellsPress, this, menu_selector(ChoiceSpells::onBtnFireSpells));
    btnFireSpells->setAnchorPoint(ccp(0, 0));
    btnFireSpells->setPosition(ccp(648, 625));
    
    btnEarthSpells = CCMenuItemSprite::create(bEarthSpells, bEarthSpellsPress, this, menu_selector(ChoiceSpells::onBtnEarthSpells));
    btnEarthSpells->setAnchorPoint(ccp(0, 0));
    btnEarthSpells->setPosition(ccp(739, 625));
    
    btnAirSpells = CCMenuItemSprite::create(bAirSpells, bAirSpellsPress, this, menu_selector(ChoiceSpells::onBtnAirSpells));
    btnAirSpells->setAnchorPoint(ccp(0, 0));
    btnAirSpells->setPosition(ccp(830, 625));
    
    this->pMenuForButton = CCMenu::create(this->btnApply, this->btnBack, this->btnWaterSpells, this->btnFireSpells, this->btnEarthSpells, this->btnAirSpells, NULL);
    this->pMenuForButton->init();
    this->pMenuForButton->setPosition(ccp(0, 0));
    
    this->addChild(pMenuForButton, 0);
    
    //Elements Icons
    iconElement = CCSprite::create("Textures/GUI/ChoiceSpells/Icon_Element.png");
    iconElement->setAnchorPoint(ccp(0, 0));
    iconElement->setPosition(ccp(550, 370));
    this->addChild(iconElement);
    
    //Select icon spells
    for(int i = 0 ;  i < 4 ; i++)
    {
        selectedSpellIcons[i] = CCSprite::create("Textures/GUI/ChoiceSpells/Selected_Border.png");
        selectedSpellIcons[i]->setAnchorPoint(ccp(0, 0));
        selectedSpellIcons[i]->setPosition(ccp(180 + i * 160, 90));
        selectedSpellIcons[i]->setZOrder(6);
        this->addChild(selectedSpellIcons[i]);
    }
    //
    
    //
    for(int i = 0, j = 0 ; i < 16 ; i++, j++)
    {
        if(j == 4)
            j = 0;
        
        //Order Icons and borders
        deck[i] = SpellCard::initWithXML("magic.xml", i);

        //Descriptions
        this->addChild(deck[i]->getSpellDescription());
        this->addChild(deck[i]->getBigIcon());
        this->addChild(deck[i]->getSelectIcon());
        deck[i]->getBody()->setAnchorPoint(ccp(0, 0));
        deck[i]->getBody()->setPosition(ccp(180, 570 - (j * 125)));
        
        this->addChild(deck[i]->getBody());
        
        deck[i]->setAvailable(Memory->spellbook[i]);
        if(deck[i]->getRequirements() < Memory->level1)
            deck[i]->setLockState(false);
        else
            deck[i]->setLockState(true);
    }
    
    btnWaterSpells->selected();
    btnFireSpells->unselected();
    btnEarthSpells->unselected();
    btnAirSpells->unselected();
    
    waterDekor->setVisible(true);
    fireDekor->setVisible(false);
    earthDekor->setVisible(false);
    airDekor->setVisible(false);
    
    for(int i = 0 ; i < 4 ; i++)
    {
        deck[Memory->chosenSpellsID1[i] - 1]->getSelectIcon()->setVisible(true);
    }
    
    showSpells(0, 4);
    
    //this->setTouchEnabled(true);
    //this->scheduleUpdate();
    
    return true;
}
//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event)
{

}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event)
{

}
//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::showSpells(int beginID, int endID)
{
    for(int i = 0, j = 0 ; i < 16 ; i++, j++)
    {
        if(i >= beginID && i < endID)
        {
            deck[i]->getBtnBorder()->setVisible(true);
            deck[i]->getIcon()->setVisible(true);
            if(deck[i]->getAvailable())
            {
                deck[i]->setSpellDisplayVisible(true);
                deck[i]->getBtnPayGold()->setVisible(false);
                deck[i]->getBtnPayCrystall()->setVisible(false);
            }
            else
            {
                deck[i]->setSpellDisplayVisible(false);
                deck[i]->getBtnPayGold()->setVisible(true);
                
                if(deck[i]->getLockState())
                {
                    deck[i]->getBtnPayCrystall()->setVisible(true);
                    deck[i]->getLockBorder()->setVisible(true);
                    deck[i]->getBtnPayGold()->setVisible(false);
                }
            }
        }
        else
        {
            deck[i]->getLockBorder()->setVisible(false);
            deck[i]->getBtnBorder()->setVisible(false);
            deck[i]->getIcon()->setVisible(false);
            deck[i]->setSpellDisplayVisible(false);
            deck[i]->getBtnPayGold()->setVisible(false);
            deck[i]->getBtnPayCrystall()->setVisible(false);
        }
        deck[i]->getSelectBorder()->setVisible(false);
        deck[i]->getBigIcon()->setVisible(false);
        deck[i]->getSpellDescription()->setVisible(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::onBtnApply(CCObject *pSender)
{
    if(!isEndScene)
    {
        for(int i = 0 ; i < 9 ; i++)
        {
            if(Memory->chosenCardsID1[i] > 0)
            {
                SpellCard::destroy();
                Card::release();
                this->setTouchEnabled(false);
                isEndScene = true;
                CCDirector::sharedDirector()->replaceScene(MyBattleScene::scene());
                break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::onBtnBack(CCObject *pSender)
{
    this->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::onBtnWaterSpells(CCObject* pSender)
{
    btnWaterSpells->selected();
    btnFireSpells->unselected();
    btnEarthSpells->unselected();
    btnAirSpells->unselected();
    
    waterDekor->setVisible(true);
    fireDekor->setVisible(false);
    earthDekor->setVisible(false);
    airDekor->setVisible(false);
    
    showSpells(0, 4);
}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::onBtnFireSpells(CCObject* pSender)
{
    btnWaterSpells->unselected();
    btnFireSpells->selected();
    btnEarthSpells->unselected();
    btnAirSpells->unselected();
    
    waterDekor->setVisible(false);
    fireDekor->setVisible(true);
    earthDekor->setVisible(false);
    airDekor->setVisible(false);
    
    showSpells(4, 8);
}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::onBtnEarthSpells(CCObject* pSender)
{
    btnWaterSpells->unselected();
    btnFireSpells->unselected();
    btnEarthSpells->selected();
    btnAirSpells->unselected();
    
    waterDekor->setVisible(false);
    fireDekor->setVisible(false);
    earthDekor->setVisible(true);
    airDekor->setVisible(false);
    
    showSpells(8, 12);
}

//---------------------------------------------------------------------------------------------------------------------------------
void ChoiceSpells::onBtnAirSpells(CCObject* pSender)
{
    btnWaterSpells->unselected();
    btnFireSpells->unselected();
    btnEarthSpells->unselected();
    btnAirSpells->selected();
    
    waterDekor->setVisible(false);
    fireDekor->setVisible(false);
    earthDekor->setVisible(false);
    airDekor->setVisible(true);
    
    showSpells(12, 16);
}