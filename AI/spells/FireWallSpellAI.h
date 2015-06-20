#ifndef __BattleMagic__FireWallSpellAI__
#define __BattleMagic__FireWallSpellAI__

#include "UnitAI.h"
#include "cocos2d.h"
//---------------------------------------------------------------------------------------------------------------------------------
class FireWallSpellAI: public UnitAI
{
public:
    static FireWallSpellAI*     createTS(BattleModel* _battleModel);
    void                        update(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits, float _dt);
    
    virtual ~FireWallSpellAI();
    
    void                        setPosition(cocos2d::CCPoint _pos);
    cocos2d::CCPoint            getPosition();
protected:
    ArtificialIntelligenceType      typeUnitAI;
    
private:
    FireWallSpellAI();
    
    bool                        canAttack(AnimatedUnitModel* attacker, AnimatedUnitModel* target, int& priority);
    
    bool                        isBodyCircleInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority);
    
    bool                        isBodyPolygonInRange(AnimatedUnitModel* _attacker, AnimatedUnitModel* _target, int _priority);
    
    void                        init(AnimatedUnitModel* _unitAI, const cocos2d::CCArray* _allUnits);
    cocos2d::CCPoint            position;
    bool                        initialize;
    float                       remainedCoolDown;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__BattleMagic__FireWallSpellAI__) */
