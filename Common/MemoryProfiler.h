#ifndef __MemoryProfiler__
#define __MemoryProfiler__

#include <vector>
#include "cocos2d.h"
#include "Enums.h"

class ICard;

//---------------------------------------------------------------------------------------------------------------------------------
class MemoryProfiler
{
public:
    static MemoryProfiler*  getInstance();
    static void             destroyInstance();
    
    //sound
    float           volumeBackground;
    float           volumeEffect;
    
    //User basic stats for HUD
    char*           name1;
    int             icon1;
    int             level1;
    int             gold1;
    int             crystal1;
    
    //Opponent basic stats fo HUD
    char*           name2;
    int             icon2;
    int             level2;
    int             gold2;
    int             crystal2;
    
    //cards
    bool            deck[48];               //User presence cards in deck
    
    int             chosenCardsID1[9];      //User chosen card to battle scene
    int             chosenCardsID2[9];      //Opponent chosen card to battle scene
    
    //spells
    bool            spellbook[16];          //User presence spell in book
    
    int             chosenSpellsID1[4];     //User chosen spell to battle scene
    int             chosenSpellsID2[4];     //Opponent chosen spell to battle scene
    
    
    //Data for battle and global map
    int                             percentMapOpen;
    int                             countQuesesCompl;
    int                             battlesWin;
    
    //card touch and click events
    ICard*            touchSpellCard;
    ICard*            clickSpellCard;
    ICard*            doubleclickSpellCard;
    ICard*            activeSpellCard;
    //
    
    float                           mapScale;
    void                            resetDateForGlobalMap();
    std::string                     battleGround;
    int                             curQuestMarker;
    int                             newQuestMarker;
    std::vector<cocos2d::CCPoint>   curRoad;
    std::vector<int>                newOpenFog;
    STATE_MARKER_HERO               stateHero;
    int                             curIndexPosOfRoad;
    int                             countWaveForFight;
    int                             countMaxEnemiesInWave;
    int                             countMinEnemiesInWave;
    int                             minDelayWaveSec;
    int                             maxDelayWaveSec;
    int                             maxDelayUnitWaveSec;
    int                             minDelayUnitWaveSec;
    std::vector<int>                enemys;
    WHENCE_START_FIGHT              whenceStartFight;
    int                             countFight;
    //
    
    //
    BACK_CARDS_SCENE                btnCardsScrBack;
    game_mode                       gameMod;
    std::string                     nameHero;
    //
    
    //for MULTIPLAYER only
    bool                            mirror;     //type display units of automatic update
    
    int                             newLevelExp;
    int                             currentExp;
    //Id avatars
    bool                            heroIcons[21];
private:
    static MemoryProfiler*          instance;
    MemoryProfiler();
    ~MemoryProfiler();
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif  //__MemoryProfiler__