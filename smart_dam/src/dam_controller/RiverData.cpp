#include "RiverData.h"
#include "Arduino.h"

RiverData::RiverData(){
    this->riverState = NORMAL;
    this->damMode = AUTO;
    this->distance = 5.0;
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

void RiverData::setDistance(float distance){
    this->distance = distance;
}

float RiverData::getDistance(){
    return this->distance;
}
