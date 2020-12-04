#include "BlinkLedTask.h"
#include "Arduino.h"

BlinkLedTask::BlinkLedTask(Experimentation* experimentation, Led* led){
    this->experimentation = experimentation;
    this->led = led;
}

void BlinkLedTask::init(int period){
    state = BL0;
    Task::init(period);
}

bool BlinkLedTask::updateTimeAndCheckEvent(int basePeriod){
    State nextState = state;
    bool result = false;
    Experimentation::State expState = this->experimentation->getExperimentationState();

    switch(state){
        case BL0:
            if(expState == Experimentation::State::ERROR ||
                expState == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                nextState = BL1;
                result = true;
                Task::init(getPeriod());
                break;
            }
            break;
        
        case BL1:
            if(expState == Experimentation::State::ERROR ||
                expState == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                    if(updateAndCheckTime(basePeriod)){
                        nextState = BL2;
                        result = true;
                        break;
                    }
                    break;
            } else {
                nextState = BL0;
                result = true;
                break;
            }
        
        case BL2:
            if(expState == Experimentation::State::ERROR ||
                expState == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                    if(updateAndCheckTime(basePeriod)){
                        nextState = BL1;
                        result = true;
                        break;
                    }
                    break;
            } else {
                nextState = BL0;
                result = true;
                break;
            }     
    }
    state = nextState;
    return result;
}

void BlinkLedTask::tick(){
    switch(state){
        case BL0:
        case BL2:
            this->led->switchOff();
            break;

        case BL1:
            this->led->switchOn();
            break;
    }
}