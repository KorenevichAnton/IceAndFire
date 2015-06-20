#include "MemoryProfiler.h"

MemoryProfiler* MemoryProfiler::instance = 0;

//---------------------------------------------------------------------------------------------------------------------------------
MemoryProfiler::MemoryProfiler():
name1(0),
icon1(0),
level1(0),
mapScale(0.0),
name2(0),
icon2(0),
level2(0),
touchSpellCard(NULL),
clickSpellCard(NULL),
doubleclickSpellCard(NULL),
mirror(false)
{
    //cards
    for(int i = 0; i < 48; i++)
        deck[i] = false;
    
    for(int i = 0; i < 9; i++)
    {
        chosenCardsID1[i] = 0;
        chosenCardsID2[i] = 0;
    }
    //

    //spells
    for(int i = 0; i < 16; i++)
        spellbook[i] = false;
    
    for(int i = 0; i < 4; i++)
    {
        chosenSpellsID1[i] = 0;
        chosenSpellsID2[i] = 0;
    }
    //
    
    //TEMP
        //TEMP: need a save progress system
        //name1 = new char[16];
        //name1 = "User";
        icon1 = 1;
        level1 = 11;
        gold1 = 10000;
        crystal1 = 10000;
    
        //TEMP: need a save progress system
        deck[0] = true;
        deck[1] = true;
        deck[2] = true;
        deck[3] = true;
    
        //TEMP: need a save progress system
        spellbook[0] = true;
        spellbook[1] = true;
        spellbook[4] = true;
        spellbook[5] = true;
        spellbook[8] = true;
        spellbook[9] = true;
        spellbook[12] = true;
        spellbook[13] = true;
    
        //TEMP: need a choisen spells window
        chosenSpellsID1[0] = 4;
        chosenSpellsID1[1] = 6;
        chosenSpellsID1[2] = 10;
        chosenSpellsID1[3] = 16;
    //
    
    //TODO load progress on enter to the IceAndFire
    
    //data for battle and global map
    curQuestMarker = -1;
    newQuestMarker = -1;
    battleGround = "";
    stateHero = (STATE_MARKER_HERO)-1;
    curIndexPosOfRoad = -1;
    countWaveForFight = -1;
    countMinEnemiesInWave = -1;
    countMaxEnemiesInWave = -1;
    maxDelayUnitWaveSec = -1;
    minDelayUnitWaveSec = -1;
    minDelayWaveSec = -1;
    maxDelayWaveSec = -1;
    whenceStartFight = WSF_MARKER;
    countFight = 0;
    btnCardsScrBack = BCSB_YN_START_QUEST;
    gameMod = GM_SINGLEPLAYER;
    //
    
    //sound
    volumeBackground = 0;
    volumeEffect = 0;
    
    
    percentMapOpen = 0;
    countQuesesCompl = 0;
    battlesWin = 0;
    nameHero = "Name:Enter your name";
    
    newLevelExp = 2000;
    currentExp = 563;
    
    
    heroIcons[0] = false;
    heroIcons[1] = false;
    heroIcons[2] = false;
    heroIcons[3] = false;
    heroIcons[4] = false;
    heroIcons[5] = true;
    heroIcons[6] = false;
    heroIcons[7] = false;
    heroIcons[8] = false;
    heroIcons[9] = false;
    heroIcons[10] = true;
    heroIcons[11] = false;
    heroIcons[12] = true;
    heroIcons[13] = false;
    heroIcons[14] = false;
    heroIcons[15] = false;
    heroIcons[16] = true;
    heroIcons[17] = false;
    heroIcons[18] = false;
    heroIcons[19] = true;
    heroIcons[20] = false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MemoryProfiler::resetDateForGlobalMap()
{
    curQuestMarker = -1;
    newQuestMarker = -1;
    
    battleGround = "";
    
    curRoad.clear();
    
    stateHero = (STATE_MARKER_HERO)-1;
    curIndexPosOfRoad = -1;
    countWaveForFight = -1;
    countMinEnemiesInWave = -1;
    countMaxEnemiesInWave = -1;
    maxDelayUnitWaveSec = -1;
    minDelayUnitWaveSec = -1;
    minDelayWaveSec = -1;
    maxDelayWaveSec = -1;
    volumeBackground = 0;
    volumeEffect = 0;
    
    
    percentMapOpen = 0;
    countQuesesCompl = 0;
    battlesWin = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------
MemoryProfiler::~MemoryProfiler()
{
    delete[] name1;
    name1 = NULL;
    
    delete[] name2;
    name2 = NULL;
    
    //data for battle and global map
    battleGround = "";
    
    curRoad.clear();
    
    newOpenFog.clear();
    
    enemys.clear();
    //
}

//---------------------------------------------------------------------------------------------------------------------------------
MemoryProfiler* MemoryProfiler::getInstance()
{
    if(!instance)
        instance = new MemoryProfiler();
    
    return instance;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MemoryProfiler::destroyInstance()
{
    delete instance;
    instance = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------