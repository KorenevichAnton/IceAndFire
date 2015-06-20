#include "LevelModel.h"
#include "xmlParser.h"
#include "Utils.h"
#include "Wave.h"
#include "BattleModel.h"
#include "MemoryProfiler.h"
#include "MyBattleScene.h"

//---------------------------------------------------------------------------------------------------------------------------------
LevelModel::LevelModel():
_model(NULL),
_waves(NULL)
{
    _waves = new cocos2d::CCArray();
}

//---------------------------------------------------------------------------------------------------------------------------------
LevelModel::~LevelModel()
{
    CC_SAFE_RELEASE(_waves);
}

//---------------------------------------------------------------------------------------------------------------------------------
LevelModel* LevelModel::createTSWithGlobalMap(BattleModel* _model)
{
    LevelModel* pRet = new LevelModel();
    if(pRet)
    {
        pRet->_model = _model;
        
        if(Settings::sharedInstance()->getGameMode() == GM_SINGLEPLAYER)
        {
            pRet->enemysAllCount = 0;
            int number = MemoryProfiler::getInstance()->countWaveForFight;
            
            for(int i = 0; i < number; i++)
            {
                Wave* wave = Wave::createTSWithGlobalMap(_model);
                CC_ASSERT(wave);
                pRet->_waves->addObject(wave);
                wave->release();
            }
        }
    }
    
    return pRet;
}
//---------------------------------------------------------------------------------------------------------------------------------
LevelModel* LevelModel::createTS(BattleModel* model, const std::string &xmlFileName)
{
    LevelModel* pRet = new LevelModel();
    if(pRet)
    {
        pRet->_model = model;
        
        if(!pRet->initWithXML(xmlFileName))
            CC_SAFE_RELEASE_NULL(pRet);
    }
    
    return pRet;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool LevelModel::initWithXML(const std::string &xmlFileName)
{
    const XMLNode lvlNode = Utils::getNodeFormXMLFile(xmlFileName, "level");
    
    if (lvlNode.isEmpty())
        return false;
    
    
    //mob waves mode
    if(Settings::sharedInstance()->getGameMode() == GM_SINGLEPLAYER)
    {
        const XMLNode & wavesNode = lvlNode.getChildNode("waves");
        if (!wavesNode.isEmpty())
        {
            this->enemysAllCount = 0;
            int number = wavesNode.nChildNode("wave");
            for(int i = 0; i < number; i++)
            {
                const XMLNode& waveNode = wavesNode.getChildNode("wave", i);
                Wave* wave = Wave::createTSWithXML(_model, waveNode);
                CC_ASSERT(wave);
                _waves->addObject(wave);
                wave->release();
            }
        }
    }
    //
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelModel::update(float dt)
{
    if(Settings::sharedInstance()->getGameMode() == GM_SINGLEPLAYER)
    {
        if(!isFinish())
        {
            Wave* wave = (Wave*) _waves->objectAtIndex(0);
            if (wave->isFinish())
                _waves->removeObjectAtIndex(0);
            else
                wave->update(dt);
        }
        else
        {
            bool flag = true;
            
            CCObject* obj;
            CCARRAY_FOREACH(_model->getUnits(), obj)
            {
                AnimatedUnitModel* unit = (AnimatedUnitModel*)obj;
                
                if(unit->getTeam() == 2 && unit->isAlive())
                {
                    flag = false;
                    break;
                }
            }
            
            if(flag)
                MyBattleScene::pSelf->onVictory();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool LevelModel::isFinish()
{
    return (_waves->count() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------