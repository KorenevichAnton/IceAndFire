#include "BulletFactory.h"
#include "xmlParser.h"
#include "Utils.h"

#include "ArrowBulletAI.h"
#include "PulsarLightBulletAI.h"
#include "PulsarEarthBulletAI.h"
#include "PulsarFireBulletAI.h"
#include "PulsarColdBulletAI.h"
#include "PulsarElectricBulletAI.h"
#include "TrapBulletAI.h"
#include "Spear1BulletAI.h"
#include "Spear2BulletAI.h"
#include "GarpunBulletAI.h"
#include "PulsarSwampBulletAI.h"
#include "BoltBulletAI.h"
#include "StoneBulletAI.h"
#include "FireWallBulletAI.h"
#include "BulletBulletAI.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* BulletFactory::createTS(int _bulletID, BattleModel* _battleModel, int _unitID)
{
    AnimatedUnitModel* bullet = NULL;
    
    //init with .xml
    std::string file = CCString::createWithFormat("bullet/bullet_%i.xml", _bulletID)->getCString();
    bullet = initWithXML(_battleModel, file, _unitID);
    
    //add behavior
    if(bullet)
    {
        bullet->pBattleModel = _battleModel;
        
        if(_bulletID == 1)
            bullet->addBehavior( ArrowBulletAI::createTS(_battleModel) );
        if(_bulletID == 2)
            bullet->addBehavior( PulsarLightBulletAI::createTS(_battleModel) );
        if(_bulletID == 3)
            bullet->addBehavior( PulsarEarthBulletAI::createTS(_battleModel) );
        if(_bulletID == 4)
            bullet->addBehavior( PulsarFireBulletAI::createTS(_battleModel) );
        if(_bulletID == 5)
            bullet->addBehavior( PulsarColdBulletAI::createTS(_battleModel) );
        if(_bulletID == 6)
            bullet->addBehavior( PulsarElectricBulletAI::createTS(_battleModel) );
        if(_bulletID == 7)
            bullet->addBehavior( TrapBulletAI::createTS(_battleModel) );
        if(_bulletID == 8)
            bullet->addBehavior( Spear1BulletAI::createTS(_battleModel) );
        if(_bulletID == 9)
            bullet->addBehavior( GarpunBulletAI::createTS(_battleModel) );
        if(_bulletID == 10)
            bullet->addBehavior( PulsarSwampBulletAI::createTS(_battleModel) );
        if(_bulletID == 11)
            bullet->addBehavior( BoltBulletAI::createTS(_battleModel) );
        if(_bulletID == 12)
            bullet->addBehavior( StoneBulletAI::createTS(_battleModel) );
        if(_bulletID == 13)
            bullet->addBehavior( FireWallBulletAI::createTS(_battleModel) );
        if(_bulletID == 14)
            bullet->addBehavior( Spear2BulletAI::createTS(_battleModel) );
        if(_bulletID == 15)
            bullet->addBehavior( BulletBulletAI::createTS(_battleModel) );
    }
    //
    
    return bullet;
}

//---------------------------------------------------------------------------------------------------------------------------------
AnimatedUnitModel* BulletFactory::initWithXML(BattleModel* _battleModel, const std::string& _xmlFileName, int _unitID)
{
    const XMLNode& unitNode = Utils::getNodeFormXMLFile(_xmlFileName, "unit");
    
    if(unitNode.isEmpty())
        return NULL;
    
    AnimatedUnitModel* bullet = new AnimatedUnitModel(_unitID);
    
    bullet->setType(AUM_TYPE_BULLET);
    bullet->setSort(BULLET_AUM);
    
    if(Utils::readString(unitNode, "skin") != "")
        bullet->setSkin( CCString::createWithFormat( "Textures/Skins/bullets/%s", Utils::readString(unitNode, "skin").c_str() )->getCString() );
    
    bullet->setSpecialID( Utils::readFloat(unitNode, "id") );
    bullet->setMovingSpeed( Utils::readFloat(unitNode, "speed") );
    bullet->setMaxHp(1);
    bullet->setHp(1);
    
    bullet->setPhysicalModel( PhysicUnitModel::createWithXML(unitNode.getChildNode("model")) );
    bullet->setState("wait1");
    
    
    const XMLNode& attackNode = unitNode.getChildNode("actions");
    if(!attackNode.isEmpty())
    {
        int number = attackNode.nChildNode("action");
        for(int i = 0; i < number; i++)
        {
            const XMLNode& attack = attackNode.getChildNode("action", i);
            if(Utils::readString(attack, "type").compare("attack") == 0)
            {
                UnitAttack* att = new UnitAttack();
                att->type = AT_ENEMY;
                att->priority = i;
                att->animName = Utils::readString(attack, "animation");
                att->dmgColor = Utils::readColor3B(attack, "actionColor");
                
                const XMLNode& rangeNode = attack.getChildNode("range");
                att->rangePoint = Utils::readPointAsAttrib(rangeNode);
                att->range = Utils::readFloat(rangeNode, "r");
                
                bullet->addAttack(att);
                
                att->release();
            }
            
            const XMLNode& animNode = unitNode.getChildNode("animations");
            if (!animNode.isEmpty())
            {
                bullet->setOriginSkinPoint(Utils::readPointAsAttrib(animNode,CCPointZero));
                int number = animNode.nChildNode("animation");
                for (int i=0; i < number; i++)
                {
                    const XMLNode& anim = animNode.getChildNode("animation", i);
                    if (!anim.isEmpty())
                    {
                        float fps =  Utils::readFloat(anim, "speedFPS",0);
                        const std::string& animName = Utils::readString(anim, "name");
                        bullet->setAnimationSpeed(animName, fps);
                    }
                }
            }
        }
    }
    
    
    return bullet;
}

//---------------------------------------------------------------------------------------------------------------------------------
