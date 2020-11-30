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
    switch(state){
        case BL0:
            if(this->experimentation->getExperimentationState() == Experimentation::State::ERROR ||
                this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                nextState = BL1;
                result = true;
                Task::init(Task::getPeriod());
                break;
            }
            break;
        case BL1:
            if(this->experimentation->getExperimentationState() == Experimentation::State::ERROR ||
                this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                    if(Task::updateAndCheckTime(basePeriod)){
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
            if(this->experimentation->getExperimentationState() == Experimentation::State::ERROR ||
                this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                    if(Task::updateAndCheckTime(basePeriod)){
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
             Serial.println("spengo  L2");

            this->led->switchOff();
            break;
        case BL1:
            this->led->switchOn();
            break;
    }
}