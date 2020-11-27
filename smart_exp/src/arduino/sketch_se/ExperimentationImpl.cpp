#include "ExperimentationImpl.h"
#include "Arduino.h"

ExperimentationImpl::ExperimentationImpl(Experimentation::State initialState){
    setExperimentationState(initialState);
}

void ExperimentationImpl::setExperimentationState(Experimentation::State expState){
    Experimentation::setExperimentationState(expState);
}

Experimentation::State ExperimentationImpl::getExperimentationState(){
    return Experimentation::getExperimentationState();
}