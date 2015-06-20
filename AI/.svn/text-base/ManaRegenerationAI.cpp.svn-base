//
//  ManaRegenerationAI.cpp
//  BattleMagic
//
//  Created by Andrey Ryabushkin on 04.03.13.
//
//

#include "ManaRegenerationAI.h"
#include "AnimatedUnitModel.h"
#include "bufAll.h"

using namespace cocos2d;

ManaRegenerationAI::ManaRegenerationAI():UnitAI(){
    
}

ManaRegenerationAI::~ManaRegenerationAI(){
    
}

void ManaRegenerationAI::update(AnimatedUnitModel* unitAI, const cocos2d::CCArray *allUnits, float dt){
    if (unitAI->isAlive()){
        unitAI->manaRegeneration(dt);
    }
}