#ifndef __BattleMagic__LevelModel__
#define __BattleMagic__LevelModel__

#include "cocos2d.h"

class BattleModel;

//---------------------------------------------------------------------------------------------------------------------------------
class LevelModel: public cocos2d::CCObject
{
protected:
    BattleModel*        _model;
    cocos2d::CCArray*   _waves;
    
    LevelModel();
    
    int                 enemysAllCount;
    
public:
    virtual ~LevelModel();
    
    static LevelModel* createTSWithGlobalMap(BattleModel* _model);
    
    static LevelModel* createTS(BattleModel* model, const std::string &xmlFileName);
    bool initWithXML(const std::string &xmlFileName);
    void update(float dt);
    bool isFinish();
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__LevelModel__) */

