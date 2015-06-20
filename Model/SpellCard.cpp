//
//  SpellCard.cpp
//  BattleMagic
//
//  Created by madmin on 1/3/14.
//
//

#include "SpellCard.h"

#include "MyBattleScene.h"
#include "Utils.h"
#include "xmlParser.h"
#include "MemoryProfiler.h"
#include "ChoiceSpellsScene.h"

USING_NS_CC;

std::vector<SpellCard*> SpellCard::spellCardDeck;

SpellCard::SpellCard():
pIcon(NULL),
pSelectIcon(NULL),
pBorder(NULL),
pLockBorder(NULL),
pCloserPanelPayGold(NULL),
pCloserPanelPayCrystall(NULL),
spellCardID(0),
spellID(0),
name(""),
description(""),
damage(0),
manaCost(0),
school(""),
requirements(0),

pSpellDescription(NULL),
pSpellDisplay(NULL),
pAttackIcon(NULL),
pBigIcon(NULL),
pManaCostIcon(NULL),
pCloserHeader(NULL)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------

SpellCard::~SpellCard()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::destroy()
{
    for(std::vector<SpellCard*>::iterator i = spellCardDeck.begin(); i < spellCardDeck.end(); i++)
    {
        if((*i))
        {
            if((*i)->getIcon())
                (*i)->getIcon()->removeFromParentAndCleanup(true);
            
            delete (*i);
            (*i) = NULL;
        }
    }
    spellCardDeck.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
SpellCard* SpellCard::initWithXML(const std::string& _xmlFileName, int spellNumber)
{
    std::string dtr = _xmlFileName;
    SpellCard* pSpellCard = NULL;
    
    const XMLNode& spellsNode = Utils::getNodeFormXMLFile("magic.xml", "spells");
    if (!spellsNode.isEmpty())
    {
        int number = spellsNode.nChildNode("spellAttack");
        
        for (int i = 0; i < number; i++)
        {
            pSpellCard = new SpellCard();
            pSpellCard->pBody = CCSprite::create("Textures/clear.png");
            spellCardDeck.push_back(pSpellCard);
            const XMLNode& attackNode = spellsNode.getChildNode("spellAttack", spellNumber);
            
            pSpellCard->spellCardID = spellNumber;
            pSpellCard->spellID = Utils::readInt(attackNode, "id");
            pSpellCard->damage = Utils::readFloat(attackNode, "damage");
            pSpellCard->school = Utils::readString(attackNode, "schoolName");
            pSpellCard->manaCost = Utils::readFloat(attackNode, "manaCost");
            pSpellCard->goldCost = Utils::readInt(attackNode, "goldCost");
            pSpellCard->crystallCost = Utils::readInt(attackNode, "crystallCost");
            pSpellCard->requirements = Utils::readInt(attackNode, "requirements");
            
            pSpellCard->pIcon = CCSprite::create(Utils::readString(attackNode, "orderIcon").c_str());
            pSpellCard->pBigIcon = CCSprite::create(Utils::readString(attackNode, "orderIcon").c_str());
            pSpellCard->pBigIcon->setAnchorPoint(ccp(0, 0));
            pSpellCard->pBigIcon->setPosition(ccp(625, 393));
            pSpellCard->pBigIcon->setScaleX(2);
            pSpellCard->pBigIcon->setScaleY(1.90);
            pSpellCard->pBigIcon->setVisible(false);
            pSpellCard->pSelectIcon = CCSprite::create(Utils::readString(attackNode, "selectIcon").c_str());
                        
            pSpellCard->pLockBorder = CCSprite::create("Textures/GUI/ChoiceSpells/Ico_Mini_Lock.png");            
            pSpellCard->createSpellDisplay();
            
            
            
            pSpellCard->pBorder = CCMenuItemImage::create("Textures/GUI/ChoiceSpells/Ico_Mini_Order.png", "Textures/GUI/ChoiceSpells/Ico_Mini_Select.png");
            pSpellCard->pBorder->setAnchorPoint(ccp(0, 0));
            pSpellCard->pBorder->setPosition(ccp(-10, -10));
            
            pSpellCard->pMenuButton = CCMenu::create(pSpellCard->pBorder, NULL);
            pSpellCard->pMenuButton->init();
            pSpellCard->pMenuButton->setAnchorPoint(ccp(0, 0));
            pSpellCard->pMenuButton->setPosition(ccp(0, 0));
            
            //menu button
            pSpellCard->btnPayGold = CCMenuItemImage::create("Textures/GUI/HUD/btn_gold.png",
                                                 "Textures/GUI/HUD/btn_gold_ondown.png");
            pSpellCard->btnPayGold->setAnchorPoint(ccp(0, 0));
            pSpellCard->btnPayGold->setPosition(ccp(85, -11));
            pSpellCard->btnPayGold->setVisible(false);
            
            CCMenu* menuGold = CCMenu::create(pSpellCard->btnPayGold, NULL);
            menuGold->init();
            menuGold->setAnchorPoint(ccp(0, 0));
            menuGold->setPosition(ccp(0, 0));
            
            //menu button pay crystall
            pSpellCard->btnPayCrystall = CCMenuItemImage::create("Textures/GUI/HUD/btn_crystall.png",
                                                     "Textures/GUI/HUD/btn_crystall_ondown.png");
            pSpellCard->btnPayCrystall->setPosition(ccp(85, -11));
            pSpellCard->btnPayCrystall->setVisible(false);
            pSpellCard->btnPayCrystall->setAnchorPoint(ccp(0, 0));
            
            CCMenu* menuCrystall = CCMenu::create();
            menuCrystall->setAnchorPoint(ccp(0, 0));
            menuCrystall->setPosition(ccp(0, 0));
            menuCrystall->addChild(pSpellCard->btnPayCrystall);

            
            
            ////////////////////////////////////////////////////////////////
            pSpellCard->getIcon()->setAnchorPoint(ccp(0, 0));
            pSpellCard->getIcon()->setPosition(ccp(0, 0));
            pSpellCard->getIcon()->setZOrder(10);
            
            pSpellCard->pSelectBorder = CCSprite::create("Textures/GUI/ChoiceSpells/Ico_Mini_Select.png");
            pSpellCard->pSelectBorder->setAnchorPoint(ccp(0, 0));
            pSpellCard->pSelectBorder->setPosition(ccp(-10, -10));
            
            pSpellCard->getLockBorder()->setAnchorPoint(ccp(0, 0));
            pSpellCard->getLockBorder()->setPosition(ccp(-10, -10));
            pSpellCard->getLockBorder()->setZOrder(12);
            pSpellCard->getLockBorder()->setVisible(false);
            
            //Icons of selected spells
            pSpellCard->getSelectIcon()->setAnchorPoint(ccp(0,0));
            pSpellCard->getSelectIcon()->setZOrder(5);
            
            if(pSpellCard->getSchool() == "water")
                pSpellCard->getSelectIcon()->setPosition(ccp(180, 90));
            if(pSpellCard->getSchool() == "fire")
                pSpellCard->getSelectIcon()->setPosition(ccp(340, 90));
            if(pSpellCard->getSchool() == "earth")
                pSpellCard->getSelectIcon()->setPosition(ccp(500, 90));
            if(pSpellCard->getSchool() == "air")
                pSpellCard->getSelectIcon()->setPosition(ccp(660, 90));
            
            pSpellCard->getSelectIcon()->setVisible(false);
            
            pSpellCard->btnPayGold->setTarget(pSpellCard, menu_selector(SpellCard::onBtnPayGold));
            pSpellCard->btnPayCrystall->setTarget(pSpellCard, menu_selector(SpellCard::onBtnPayCrystall));
            pSpellCard->pBorder->setTarget(pSpellCard, menu_selector(SpellCard::onBtnSelect));
            
            //Descriptions
            pSpellCard->pBody->addChild(pSpellCard->pIcon);
            pSpellCard->pBody->addChild(pSpellCard->pMenuButton, 100);
            pSpellCard->pBody->addChild(pSpellCard->pSelectBorder, 101);
            pSpellCard->pBody->addChild(pSpellCard->pLockBorder, 102);
            pSpellCard->pCloserPanelPayGold->addChild(menuGold, 103);
            pSpellCard->pCloserPanelPayCrystall->addChild(menuCrystall, 104);
            ////////////////////////////////////////////////////////////////
            break;
        }
    }
    return pSpellCard;
}

//---------------------------------------------------------------------------------------------------------------------------------

void SpellCard::createSpellDisplay()
{
    
    const XMLNode& spellsNode = Utils::getNodeFormXMLFile("magic.xml", "spells");
    std::string spellDamage;
    std::string spellMP;
    std::string spellSchool;
    
    if (!spellsNode.isEmpty())
    {
        int number = spellsNode.nChildNode("spellAttack");
        
        for (int i = 0; i < number; i++)
        {
            
            const XMLNode& attackNode = spellsNode.getChildNode("spellAttack", i);
            if(Utils::readInt(attackNode, "id") == spellID)
            {
                spellDamage = Utils::readString(attackNode, "damage");
                this->name = Utils::readString(attackNode, "name");
                spellMP = Utils::readString(attackNode, "manaCost");
                spellSchool = Utils::readString(attackNode, "schoolName");
                this->description = Utils::readString(attackNode, "description");
            }
        }
    }
    //layer displaying detail information
    pSpellDisplay = CCLayer::create();
    pSpellDisplay->setAnchorPoint(ccp(0, 0));
    pSpellDisplay->setPosition(ccp(0, 0));
    pSpellDisplay->setZOrder(-10);
    
    
    
    pCloserHeader = CCLabelTTF::create(this->name.c_str(), "Arial", 20);
    pCloserHeader->setAnchorPoint(ccp(0.5, 0.5));
    pCloserHeader->setPosition(ccp(160, 90));
    pIcon->addChild(pCloserHeader);
    //labelCloserHeader = NULL;
    //
    CCLabelTTF* labelSpellDamage = CCLabelTTF::create(spellDamage.c_str(), "Arial", 16);
    labelSpellDamage->setAnchorPoint(ccp(0.5, 0.5));
    labelSpellDamage->setPosition(ccp(160, 60));
    pSpellDisplay->addChild(labelSpellDamage);
    //
    CCLabelTTF* labelSpellManaCost = CCLabelTTF::create(spellMP.c_str(), "Arial", 16);
    labelSpellManaCost->setAnchorPoint(ccp(0.5, 0.5));
    labelSpellManaCost->setPosition(ccp(160, 30));
    pSpellDisplay->addChild(labelSpellManaCost);
    //
    CCSprite* icon = NULL;
    
    if(getSchool() == "water")
        icon = CCSprite::create("Textures/GUI/ChoiceSpells/miniCost_water.png");
    if(getSchool() == "fire")
        icon = CCSprite::create("Textures/GUI/ChoiceSpells/miniCost_fire.png");
    if(getSchool() == "earth")
        icon = CCSprite::create("Textures/GUI/ChoiceSpells/miniCost_earth.png");
    if(getSchool() == "air")
        icon = CCSprite::create("Textures/GUI/ChoiceSpells/miniCost_air.png");
    if(icon)
    {
        icon->setAnchorPoint(ccp(0, 0));
        icon->setPosition(ccp(110, 15));
        pSpellDisplay->addChild(icon);
    }
    CCSprite* attackIcon = NULL;
    attackIcon = CCSprite::create("Textures/GUI/ChoiceSpells/attack.png");
    attackIcon->setAnchorPoint(ccp(0, 0));
    attackIcon->setPosition(ccp(110, 50));
    pSpellDisplay->addChild(attackIcon);
    attackIcon = NULL;
    
    //panel pay gold
    
    pCloserPanelPayGold = CCSprite::createWithSpriteFrameName("_0020_info_window_btn.png");
    pCloserPanelPayGold->setAnchorPoint(ccp(0, 0));
    pCloserPanelPayGold->setPosition(ccp(100, -5));
    pIcon->addChild(pCloserPanelPayGold);
    /*
    btnPayGold = CCMenuItemImage::create("Textures/GUI/HUD/btn_gold.png",
                                         "Textures/GUI/HUD/btn_gold_ondown.png");
    btnPayGold->setAnchorPoint(ccp(0, 0));*/
    
        //"buy for" text label
        CCLabelTTF* labelBuy1 = CCLabelTTF::create("Buy for", "Arial", 20);
        labelBuy1->setAnchorPoint(ccp(0.5, 0.5));
        labelBuy1->setPosition(ccp(55, 45));
        pCloserPanelPayGold->addChild(labelBuy1);
        labelBuy1 = NULL;
        //price text label
        char* strGold = new char[8];
        sprintf(strGold, "%d", goldCost);
    
        CCLabelTTF* labelCloserPriceGold = CCLabelTTF::create(strGold, "Arial", 20);
        labelCloserPriceGold->setAnchorPoint(ccp(1, 0.5));
        labelCloserPriceGold->setPosition(ccp(89, 20));
        pCloserPanelPayGold->addChild(labelCloserPriceGold);
        labelCloserPriceGold = NULL;
    
        delete[] strGold;
        strGold = NULL;
    
    pCloserPanelPayGold->setVisible(false);
    //
    
    //panel pay crystall
    
    pCloserPanelPayCrystall = CCSprite::createWithSpriteFrameName("_0020_info_window_btn.png");
    pCloserPanelPayCrystall->setAnchorPoint(ccp(0, 0));
    pCloserPanelPayCrystall->setPosition(ccp(100, -5));
    pIcon->addChild(pCloserPanelPayCrystall);
    
    btnPayCrystall = CCMenuItemImage::create("Textures/GUI/HUD/btn_crystall.png",
                                             "Textures/GUI/HUD/btn_crystall_ondown.png");
    btnPayCrystall->setAnchorPoint(ccp(0, 0));
    
    //"buy for" text label
    CCLabelTTF* labelBuy2 = CCLabelTTF::create("Unlock for", "Arial", 20);
    labelBuy2->setAnchorPoint(ccp(0.5, 0.5));
    labelBuy2->setPosition(ccp(55, 45));
    pCloserPanelPayCrystall->addChild(labelBuy2);
    
    //price text label
    char* strCrystall = new char[8];
    sprintf(strCrystall, "%d", crystallCost);
    
    CCLabelTTF* labelCloserPriceCrystall = CCLabelTTF::create(strCrystall, "Arial", 20);
    labelCloserPriceCrystall->setAnchorPoint(ccp(1, 0.5));
    labelCloserPriceCrystall->setPosition(ccp(89, 20));
    pCloserPanelPayCrystall->addChild(labelCloserPriceCrystall);
    
    delete[] strCrystall;
    strCrystall = NULL;
    //
    pSpellDescription = CCLayer::create();
    pSpellDescription->setAnchorPoint(ccp(0,0));
    pSpellDescription->setPosition(ccp(670, 375));
    pSpellDescription->setVisible(false);
    CCLabelTTF* labelSpellDescriptionName = CCLabelTTF::create(this->name.c_str(), "Arial", 20);
    labelSpellDescriptionName->setAnchorPoint(ccp(0.5, 0.5));
    labelSpellDescriptionName->setPosition(ccp(50, -20));
    pSpellDescription->addChild(labelSpellDescriptionName);
    
    CCLabelTTF* labelSpellDescription = CCLabelTTF::create(this->description.c_str(), "Arial", 14, CCSizeMake(300, 200), kCCTextAlignmentCenter);
    labelSpellDescription->setAnchorPoint(ccp(0.5, 0.5));
    labelSpellDescription->setPosition(ccp(50, -150));
    pSpellDescription->addChild(labelSpellDescription);
    labelSpellDescription = NULL;
    //pSpellDisplay->addChild(labelCloserHeader);
    
    pIcon->addChild(pSpellDisplay);
}

//---------------------------------------------------------------------------------------------------------------------------------

void SpellCard::setSpellDisplayVisible(const bool _state)
{
    pSpellDisplay->setVisible(_state);
}

//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::setLockState(const bool _state)
{
    lock = _state;
    pCloserPanelPayCrystall->setVisible(_state);
    btnPayCrystall->setVisible(_state);
    if(_state && pCloserHeader)
    {
        pMenuButton->setTouchEnabled(false);
        pCloserPanelPayGold->setVisible(!_state);
        pCloserHeader->setString("Unknown");
    }else
    {
        pCloserHeader->setString(this->name.c_str());
        pMenuButton->setTouchEnabled(true);
        pLockBorder->setVisible(false);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::setAvailable(const bool _state)
{
    available = _state;
    pSpellDisplay->setVisible(_state);
    pCloserPanelPayGold->setVisible(!_state);
}

//---------------------------------------------------------------------------------------------------------------------------------
bool SpellCard::getSpellDisplayVisible() const
{
    return pSpellDisplay->isVisible();
}

//---------------------------------------------------------------------------------------------------------------------------------
CCSprite* SpellCard::getIcon() const
{
    if(pIcon)
        return pIcon;
    
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
CCSprite* SpellCard::getBigIcon() const
{
    if(pBigIcon)
        return pBigIcon;
    
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
CCSprite* SpellCard::getSelectIcon() const
{
    if(pSelectIcon)
        return pSelectIcon;
    
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
CCSprite* SpellCard::getSelectBorder() const
{
    if(pSelectBorder)
        return pSelectBorder;
    
    return NULL;
}
//---------------------------------------------------------------------------------------------------------------------------------
CCMenuItemImage* SpellCard::getBtnBorder() const
{
    if(pBorder)
        return pBorder;
    
    return NULL;
}
//---------------------------------------------------------------------------------------------------------------------------------
CCSprite* SpellCard::getLockBorder() const
{
    if(pLockBorder)
        return pLockBorder;
    
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
CCLayer* SpellCard::getSpellDescription() const
{
    if(pSpellDescription)
        return pSpellDescription;
    
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
int SpellCard::getSpellID() const
{
    return spellID;
}

//---------------------------------------------------------------------------------------------------------------------------------
int SpellCard::getSpellCardID() const
{
    return spellCardID;
}

//---------------------------------------------------------------------------------------------------------------------------------
std::string SpellCard::getSchool() const
{
    return school;
}

//---------------------------------------------------------------------------------------------------------------------------------
int SpellCard::getRequirements() const
{
    if(requirements)
        return requirements;
    
    return NULL;
}
//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::onBtnPayGold(cocos2d::CCObject* pSender)
{
    if(Memory->gold1 >= this->getGoldCost() && this->getRequirements() < Memory->level1)// && !deck[i]->getAvailable())
    {
        Memory->gold1 -= this->getGoldCost();
        Memory->spellbook[this->getSpellCardID()] = true;
        
        this->getBtnPayGold()->setVisible(false);
        this->setAvailable(true);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::onBtnPayCrystall(cocos2d::CCObject* pSender)
{
    if(Memory->crystal1 >= this->getCrystalCost())// && !deck[i]->getAvailable())
    {
        Memory->crystal1 -= this->getCrystalCost();
        Memory->spellbook[this->getSpellCardID()] = true;
        this->setLockState(false);
        this->setAvailable(true);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::selectSpellFromDeck()
{
    MemoryProfiler* memory = MemoryProfiler::getInstance();
    if(dynamic_cast<SpellCard*>(MemoryProfiler::getInstance()->activeSpellCard)->getSchool() == "water")
    {
        memory->chosenSpellsID1[0] = dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSpellID();
        for(int i = 0 ; i < 4 ; i++)
        {
            spellCardDeck[i]->getSelectIcon()->setVisible(false);
        }
    }
    if(dynamic_cast<SpellCard*>(MemoryProfiler::getInstance()->activeSpellCard)->getSchool() == "fire")
    {
        memory->chosenSpellsID1[1] = dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSpellID();
        for(int i = 4 ; i < 8 ; i++)
        {
            spellCardDeck[i]->getSelectIcon()->setVisible(false);
        }
    }
    if(dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSchool() == "earth")
    {
        memory->chosenSpellsID1[2] = dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSpellID();
        for(int i = 8 ; i < 12 ; i++)
        {
            spellCardDeck[i]->getSelectIcon()->setVisible(false);
        }
    }
    if(dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSchool() == "air")
    {
        memory->chosenSpellsID1[3] = dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSpellID();
        for(int i = 12 ; i < 16 ; i++)
        {
            spellCardDeck[i]->getSelectIcon()->setVisible(false);
        }
    }
    spellCardDeck[dynamic_cast<SpellCard*>(memory->activeSpellCard)->getSpellCardID()]->getSelectIcon()->setVisible(true);
}
//---------------------------------------------------------------------------------------------------------------------------------
void SpellCard::onBtnSelect(CCObject* pSender)
{
    for (std::vector<SpellCard*>::iterator iter = spellCardDeck.begin(); iter < spellCardDeck.end(); iter++)
    {
        if((*iter)->getSpellCardID() != this->getSpellCardID())
        {
            (*iter)->pSelectBorder->setVisible(false);
            (*iter)->pBigIcon->setVisible(false);
            (*iter)->pSpellDescription->setVisible(false);
        }
    }
    MemoryProfiler* memory = MemoryProfiler::getInstance();
    memory->activeSpellCard = this;
    memory->touchSpellCard = this;
            if(memory->clickSpellCard == memory->activeSpellCard)
            {
                memory->doubleclickSpellCard = memory->activeSpellCard;
                this->selectSpellFromDeck();
            }
            else
            {
                memory->clickSpellCard = memory->activeSpellCard;
                memory->doubleclickSpellCard = NULL;
            }

    this->pSelectBorder->setVisible(true);
    this->pBigIcon->setVisible(true);
    this->pSpellDescription->setVisible(true);
}