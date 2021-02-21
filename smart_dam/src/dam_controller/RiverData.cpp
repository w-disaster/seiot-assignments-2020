#include "RiverData.h"
#include "Arduino.h"

RiverData::RiverData(){
    this->riverState = NORMAL;
    this->damMode = AUTO;
    this->damOpening = 0;
}

void RiverData::setRiverState(RiverData::RiverState riverState){
    this->riverState = riverState;
}

RiverData::RiverState RiverData::getRiverState(){
   return this->riverState;
}

void RiverData::setDamMode(RiverData::DamMode damMode){
    this->damMode = damMode;
}

RiverData::DamMode RiverData::getDamMode(){
    return this->damMode;
}

void RiverData::setDamOpening(int damOpening){
    this->damOpening = damOpening;
}

int RiverData::getDamOpening(){
    return this->damOpening;
}
