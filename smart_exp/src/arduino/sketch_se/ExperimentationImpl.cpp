#include "ExperimentationImpl.h"
#include "Arduino.h"

ExperimentationImpl::ExperimentationImpl(ExperimentationImpl::State initialState){
    setExperimentationState(initialState);
}

void ExperimentationImpl::setExperimentationState(ExperimentationImpl::State expState){
    Experimentation::setExperimentationState(expState);
}

ExperimentationImpl::State ExperimentationImpl::getExperimentationState(){
    return Experimentation::getExperimentationState();
}

void ExperimentationImpl::setDistance(float x){
    Experimentation::setDistance(x);
}

float ExperimentationImpl::getDistance(){
    return Experimentation::getDistance();
}

void ExperimentationImpl::setSpeed(float v){
    Experimentation::setSpeed(v);
}

float ExperimentationImpl::getSpeed(){
    return Experimentation::getSpeed();
}

void ExperimentationImpl::setAcceleration(float a){
    Experimentation::setAcceleration(a);
}

float ExperimentationImpl::getAcceleration(){
    return Experimentation::getAcceleration();
}