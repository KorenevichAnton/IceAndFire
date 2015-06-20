#ifndef BattleMagic_Enums_h
#define BattleMagic_Enums_h

//Settings

enum game_mode{
    GM_NONE = 0,
    GM_SINGLEPLAYER = 1,
    GM_SINGLE_AI = 2,
    GM_MULTIPLAYER = 3,
    GM_GLOBAL_MAP = 4
};

enum bms_show_magic_type{
    BMS_SHOW_PNG_MAGIC = 0,
    BMS_SHOW_PARTICLE_MAGIC =1
};

//Programm entities
enum UnitModelLookMode{
    AUM_LOOK_RIGHT_MODE = 0,
    AUM_LOOK_LEFT_MODE = 1
};

// diffirent part of full movment
enum UnitModelMovingMode{
    AUM_MOVE_MODE_NONE = 0,
    // move to the point
    AUM_MOVE_MODE_POINT =1,
    // move by the direction
    AUM_MOVE_MODE_DIRECTION =2
    
};

enum UnitModelType{
    AUM_TYPE_CHAR = 0,
    AUM_TYPE_NONE = 1,
    AUM_TYPE_MOB = 2,
    AUM_TYPE_BULLET = 3,
    AUM_TYPE_SPELL = 4
};

enum STATE_MARKER_HERO {
    SMH_FIGHT = 0,
    SMH_MOVE = 1,
    SMH_WAIT = 2,
    SMH_ARRIVED = 3,
    SMH_RETREAT = 4,
    SMH_QUEST = 5,
    SMH_LOOSE = 6,
    SMH_WIND_HERO = 7
};

enum WHENCE_START_FIGHT {
    WSF_MARKER = 0,
    WSF_ROAD = 1,
};

enum BACK_CARDS_SCENE{
    BCSB_YN_START_QUEST = 0,
    BCSB_YN_START_FIGHT = 1,
    BCSB_WIND_HERO = 2
};


#endif
