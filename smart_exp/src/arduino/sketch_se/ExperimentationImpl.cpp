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