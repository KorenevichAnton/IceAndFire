#include "MovingUnitAI.h"
#include "AnimatedUnitModel.h"
#include "Settings.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
MovingUnitAI::MovingUnitAI():UnitAI()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
MovingUnitAI::~MovingUnitAI()
{
 
}

//---------------------------------------------------------------------------------------------------------------------------------
void MovingUnitAI::update(AnimatedUnitModel* unitAI,const cocos2d::CCArray* allUnits, float dt)
{
    //CCObject* obj;

    if (unitAI->isAlive())
    {
        if (unitAI->getMoveMode() != AUM_MOVE_MODE_NONE)
        {
            if (unitAI->getMoveMode() == AUM_MOVE_MODE_DIRECTION)
                unitAI->setPosition(ccpAdd(unitAI->getPosition(), ccpMult(unitAI->getMoveDirection(),unitAI->getMovingSpeed()*dt)));
            
            if ((unitAI->getMoveMode() == AUM_MOVE_MODE_POINT) &&
                (unitAI->getMovingSpeed() != 0))
            {
                if (ccpDistance(unitAI->getPosition(), unitAI->getMoveTarget()) > 0)
                {
                    unitAI->setState("move1");
                    
                    
                    float part =  unitAI->getMovingSpeed()*dt/ccpDistance(unitAI->getMoveTarget(), unitAI->getPosition());
                    if (part < 1)
                    {
                        CCPoint resV = ccpMult(ccpSub( unitAI->getMoveTarget(),unitAI->getPosition()),part);
                        

                        if (resV.x >0)
                            unitAI->setLookMode(AUM_LOOK_LEFT_MODE);
                        else
                            unitAI->setLookMode(AUM_LOOK_RIGHT_MODE);
                        
                        unitAI->setPosition(ccpAdd(unitAI->getPosition(),resV));
                    }
                    else
                    {
                        unitAI->setPosition(unitAI->getMoveTarget());
                        unitAI->setState("wait1");
                    }
                    
                }
                else
                {
                    unitAI->setState("wait1");
                }
            }
            
            
            // border controll
            if (!Settings::sharedInstance()->getDeathRect().containsPoint(unitAI->getPosition()))
                unitAI->damage(unitAI->getAttack(), unitAI->getHp());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------