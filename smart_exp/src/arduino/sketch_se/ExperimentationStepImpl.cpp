#include "ExperimentationStepImpl.h"
#include "Arduino.h"

ExperimentationStepImpl::ExperimentationStepImpl(ExperimentationStepImpl::Step initialStep){
    setStep(initialStep);
}

void ExperimentationStepImpl::setStep(ExperimentationStepImpl::Step expStep){
    ExperimentationStep::setStep(expStep);
}

ExperimentationStepImpl::Step ExperimentationStepImpl::getStep(){
    return ExperimentationStep::getStep();
}