#include "KinematicsDataImpl.h"
#include "Arduino.h"

KinematicsDataImpl::KinematicsDataImpl(){
    setDataReady(false);
}

void KinematicsDataImpl::setDistance(float x){
    KinematicsData::setDistance(x);
}

float KinematicsDataImpl::getDistance(){
    return KinematicsData::getDistance();
}

void KinematicsDataImpl::setSpeed(float v){
    KinematicsData::setSpeed(v);
}

float KinematicsDataImpl::getSpeed(){
    return KinematicsData::getSpeed();
}

void KinematicsDataImpl::setAcceleration(float a){
    KinematicsData::setAcceleration(a);
}

float KinematicsDataImpl::getAcceleration(){
    return KinematicsData::getAcceleration();
}

void KinematicsDataImpl::setDataReady(bool flag){
    KinematicsData::setDataReady(flag);
}

bool KinematicsDataImpl::isDataReady(){
    return KinematicsData::isDataReady();
}