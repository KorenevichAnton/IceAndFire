#ifndef __BattleMagic__MyBattleScene__
#define __BattleMagic__MyBattleScene__

#include "cocos2d.h"
#include "Settings.h"
#include "HUDScreen.h"
#include "BattleModel.h"
#include "BattleListenerInterface.h"
#include "Card.h"
#include "CCheckpoint.h"
//---------------------------------------------------------------------------------------------------------------------------------
class MyBattleScene: public cocos2d::CCLayer, public BattleListenerInterface
{
public:
    static MyBattleScene*       pSelf;
    
    static cocos2d::CCScene*    scene();
    static MyBattleScene*       create();

    virtual ~MyBattleScene();
    //receive network events, for only MULTIPLAYER game
    void onUnitUpdate(int _unitID, int _posX, int _posY);
    void onChangeState(int _unitID, const char* _state);
    void onChangeDirection(int _unitID);
    void onChangeHp(int _unitID, float _hp);
    void onShowMessage(int _posX, int _posY, const char* _msg, const char* _color);
    void onCreateMob(int _unitID, int _invokerID, int _mobID, int _dir);
    void onCreateSpell(int _unitID, int _invokerID, int _spellID, int _dir);
    void onCreateBullet(int _unitID, int _invokerID, int _bulletID, int _dir);
    void onCreateAnimation(int _unitID, int _invokerID, const char* _skin, int _dir);
    void onRemoveUnit(int _unitID);
    void onCardApplySuccess(int _cardID);
    void onVictory();
    void onDefeat();
    void onExit();
    //

    cocos2d::CCSprite**    getZoneCP();
    cocos2d::CCSprite**    getZoneFromCPtoCP();

    //Checkpoint
    std::vector<CCheckpoint*>&  getCheckpoints();
    void                        initCheckpointsWithXML();
    ////////////
    
private:
    MyBattleScene();
    
    BattleModel*            pBattleModel;
    cocos2d::CCLayerColor*  pUnitsLayer;
    cocos2d::CCSprite*      pBackground;
    
    bool            init();
    
    void            update(float _dt);
    
    void            replaceSceneCallback();
    
    
    void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    
    
    void addUnit(AnimatedUnitModel* unit);
    void removeUnit(const AnimatedUnitModel*  unit);
    void showMessage(AnimatedUnitModel* _unit, const char* _msg, const cocos2d::ccColor3B _color, cocos2d::CCPoint _offset = cocos2d::CCPoint(0,0));
    void unitReordering();
    
    int  getActiveSpellId();
    
    int  getTouchesLine(const cocos2d::CCPoint _position) const;
    int  getSituationLine(const cocos2d::CCPoint _position) const;  //for new conception of gameplay
    int  getMinionPosition(int line); //for new conception of gameplay
    
    void createSpell(const cocos2d::CCPoint& _touch);
    void MonsterSummon(int mobID, cocos2d::CCPoint position);
    
    
    inline Character*       getUserHero() const      { return pBattleModel->getUserHero(); };
    inline Character*       getOpponentHero() const  { return pBattleModel->getOpponentHero(); };
    
    //cards
    Card*                 cards[9];
    Card*                 activeCard;
    cocos2d::CCPoint      activeCardDefaultPos;
    cocos2d::CCPoint      activeCardClickOffset;
    
    //clots mana
    void generateClotMana();
    int  chance[4];
    
    //dispersion mobs by Y on lines
    int dispersionMobs[3];
    
    
    //for MULTIPLAYER game mode
    void addHeroesForUsers();
    
    void removeMessageCallback(void* _obj);
    //
    
    bool isEndScene;
    
    //backlight. Anton
    cocos2d::CCSprite* spawningZones[5];
    cocos2d::CCSprite* zoneFromCPtoCP[4];
    cocos2d::CCSprite* zoneCP[5];
    
    //Checkpoint
    std::vector<CCheckpoint*>   vCheckpoints;
    //
    
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__MyBattleScene__) */
