#include "BlinkLedTask.h"
#include "Arduino.h"

BlinkLedTask::BlinkLedTask(ExperimentationStep* experimentationStep, Led* led){
    this->experimentationStep = experimentationStep;
    this->led = led;
}

void BlinkLedTask::init(int period){
    step = BL0;
    Task::init(period);
}

bool BlinkLedTask::updateTimeAndCheckEvent(int basePeriod){
    Step nextStep = step;
    bool result = false;
    ExperimentationStep::Step expStep = this->experimentationStep->getStep();

    switch(step){
        case BL0:
            if(expStep == ExperimentationStep::Step::ERROR ||
                expStep == ExperimentationStep::Step::EXPERIMENTATION_CONCLUDED){
                nextStep = BL1;
                result = true;
                Task::init(getPeriod());
                break;
            }
            break;
        
        case BL1:
            if(expStep == ExperimentationStep::Step::ERROR ||
                expStep == ExperimentationStep::Step::EXPERIMENTATION_CONCLUDED){
                    if(updateAndCheckTime(basePeriod)){
                        nextStep = BL2;
                        result = true;
                        break;
                    }
                    break;
            } else {
                nextStep = BL0;
                result = true;
                break;
            }
        
        case BL2:
            if(expStep == ExperimentationStep::Step::ERROR ||
                expStep == ExperimentationStep::Step::EXPERIMENTATION_CONCLUDED){
                    if(updateAndCheckTime(basePeriod)){
                        nextStep = BL1;
                        result = true;
                        break;
                    }
                    break;
            } else {
                nextStep = BL0;
                result = true;
                break;
            }     
    }
    step = nextStep;
    return result;
}

void BlinkLedTask::tick(){
    switch(step){
        case BL0:
        case BL2:
            this->led->switchOff();
            break;

        case BL1:
            this->led->switchOn();
            break;
    }
}