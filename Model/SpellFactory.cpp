#include "SpellFactory.h"
#include "Settings.h"
#include "Character.h"
#include "bufAll.h"

#include "HUDScreen.h"
#include "BattleModel.h"

//water school
#include "WaterArmorSpellAI.h"
#include "WaterNeedleSpellAI.h"
#include "WaterHealSpellAI.h"
#include "WaterRainSpellAI.h"

//fire school
#include "FireSwordSpellAI.h"
#include "FireBallSpellAI.h"
#include "FireWallSpellAI.h"
#include "FireRainSpellAI.h"

//earth school
#include "EarthPowerSpellAI.h"
#include "EarthWallSpellAI.h"
#include "EarthSwarmSpellAI.h"
#include "EarthHolySpellAI.h"

//air school
#include "AirTwistSpellAI.h"
#include "AirSpeedSpellAI.h"
#include "AirLightningSpellAI.h"
#include "AirChainSpellAI.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
SpellFactory::~SpellFactory()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
int ParticleSpellFactory::getTouchesLine(const cocos2d::CCPoint position) const
{
    int line = 0;
    
    if(position.y == 25)
        line = 1;
    
    if(position.y == 115)
        line = 2;
    
    if(position.y == 205)
        line = 3;
    
    return line;
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* PngSequenceSpellFactory::createTSSpell(spellAttack* attack,
                                                          const cocos2d::CCPoint& point,
                                                          const cocos2d::CCPoint& casterPoint,
                                                          cocos2d::ccColor3B color,
                                                          int _unitID)const
{
    AnimatedUnitModel* spell = NULL;
    
    return spell;
}

//---------------------------------------------------------------------------------------------------------------------------------
ParticleSpellFactory* ParticleSpellFactory::createTS(BattleModel* commands)
{
    ParticleSpellFactory * pRet = new ParticleSpellFactory();
    
    if (pRet && pRet->init(commands))
    {
        pRet->_commands = commands;
        return pRet;
    }
    else
    {
        delete pRet;
        return NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool ParticleSpellFactory::init(BattleModel* commands)
{
    _commands = commands;
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
ParticleSpellFactory::ParticleSpellFactory()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* ParticleSpellFactory::createTSSpell(spellAttack* attack,
                                                       const cocos2d::CCPoint& targetPoint,
                                                       const cocos2d::CCPoint& casterPoint,
                                                       cocos2d::ccColor3B color,
                                                       int _unitID) const
{
    AnimatedUnitModel* spell = NULL;
    
    if((attack->schoolName == "fire")  ||
       (attack->schoolName == "air")   ||
       (attack->schoolName == "earth") ||
       (attack->schoolName == "water") ||
       (attack->schoolName == "none"))
    {
        spell = new AnimatedUnitModel(_unitID);
        
        spell->pBattleModel = _commands;
        spell->setType(AUM_TYPE_SPELL);
        spell->setSkin(attack->skin);
        spell->setOriginSkinPoint(attack->emmiterOffset);
        spell->setAnimationSpeed("wait1", 10);  //TEMP
        spell->setAnimationSpeed("death1", 10); //TEMP
        spell->setState("wait1");
        
        spell->setLifeTime(attack->lifetime);
        
        CCPoint dir = ccpNormalize(ccpSub(targetPoint, casterPoint));
        if(dir.equals(CCPointZero))
            dir = ccp(1, 0);
        
        spell->setMoveDirection(dir);
        spell->setLookMode( (dir.x > 0) ? AUM_LOOK_LEFT_MODE : AUM_LOOK_RIGHT_MODE );
        
        spell->setMovingSpeed(attack->speed);
        
        spell->setMaxHp(1);
        spell->setHp(1);
        
        spell->setPosition(casterPoint);
        spell->setLine(getTouchesLine(casterPoint));
        
        
        //add attack
        UnitAttack* att = new UnitAttack();
        att->type = AT_ENEMY;
        att->priority = 0;
        att->schoolName = attack->schoolName;
        att->manaCost = attack->manaCost;
        att->damage = attack->damage;
        att->damageTime = attack->damageTime;
        att->damageDelta = attack->damageDelta;
        att->range = attack->range;
        att->rangePoint = attack->rangePoint;
        
        //add bufs
        CCObject* obj;
        CCARRAY_FOREACH(attack->_bufs, obj)
        {
            Buf* buf = (Buf*)obj;
            if(buf->getKind() == BUF_SELF)
                spell->addBuf(buf);
            
            if(buf->getKind() == BUF_SEND)
            {
                Buf* newcopy = buf->copy();
                att->_bufs->addObject(newcopy);
                newcopy->release();
            }
        }
        //
        
        att->dmgColor.r = color.r;
        att->dmgColor.g = color.g;
        att->dmgColor.b = color.b;
        
        spell->addAttack(att);
        
        att->release();
        
        
        //add behavior
        if(attack->id == 1)
        {
            WaterArmorSpellAI* waAI = WaterArmorSpellAI::createTS(_commands);
            waAI->setPosition(targetPoint);
            spell->addBehavior(waAI);
        }
        if(attack->id == 2)
        {
            WaterNeedleSpellAI* wnAI = WaterNeedleSpellAI::createTS(_commands);
            spell->addBehavior(wnAI);
        }
        if(attack->id == 3)
        {
            WaterHealSpellAI* whAI = WaterHealSpellAI::createTS(_commands);
            whAI->setPosition(targetPoint);
            spell->addBehavior(whAI);
        }
        if(attack->id == 4)
        {
            WaterRainSpellAI* wrAI = WaterRainSpellAI::createTS(_commands);
            wrAI->setPosition(targetPoint);
            spell->addBehavior(wrAI);
        }
        if(attack->id == 5)
        {
            FireSwordSpellAI* fsAI = FireSwordSpellAI::createTS(_commands);
            fsAI->setPosition(targetPoint);
            spell->addBehavior(fsAI);
        }
        if(attack->id == 6)
        {
            FireWallSpellAI* fw = FireWallSpellAI::createTS(_commands);
            fw->setPosition(targetPoint);
            spell->setPosition(ccp(-1000, spell->getPosition().y));
            spell->addBehavior(fw);
        }
        if(attack->id == 7)
        {
            spell->addBehavior( FireBallSpellAI::createTS(_commands) );
        }
        if(attack->id == 8)
        {
            FireRainSpellAI* frAI = FireRainSpellAI::createTS(_commands);
            frAI->setPosition(targetPoint);
            spell->addBehavior(frAI);
        }
        if(attack->id == 9)
        {
            EarthPowerSpellAI* epAI = EarthPowerSpellAI::createTS(_commands);
            epAI->setPosition(targetPoint);
            spell->addBehavior(epAI);
        }
        if(attack->id == 10)
        {
            EarthWallSpellAI* ew = EarthWallSpellAI::createTS(_commands);
            ew->setPosition(targetPoint);
            spell->setPosition(ccp(-1000, spell->getPosition().y));
            spell->addBehavior(ew);
        }
        if(attack->id == 11)
            spell->addBehavior( EarthSwarmSpellAI::createTS(_commands) );
        if(attack->id == 12)
        {
            EarthHolySpellAI* ehAI = EarthHolySpellAI::createTS(_commands);
            ehAI->setPosition(targetPoint);
            spell->addBehavior(ehAI);
        }
        if(attack->id == 13)
        {
            AirSpeedSpellAI* asAI = AirSpeedSpellAI::createTS(_commands);
            asAI->setPosition(targetPoint);
            spell->addBehavior(asAI);
        }
        if(attack->id == 14)
        {
            spell->addBehavior( AirLightningSpellAI::createTS(_commands) );
        }
        if(attack->id == 15)
        {
            AirTwistSpellAI* atAI = AirTwistSpellAI::createTS(_commands);
            atAI->setPosition(targetPoint);
            spell->addBehavior(atAI);
        }
        if(attack->id == 16)
            spell->addBehavior( AirChainSpellAI::createTS(_commands) );
    }
    
    return spell;
}

//---------------------------------------------------------------------------------------------------------------------------------