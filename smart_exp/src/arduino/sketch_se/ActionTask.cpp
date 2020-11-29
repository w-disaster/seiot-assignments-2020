#include "ActionTask.h"
#include <Arduino.h>
#include <avr/sleep.h>

ActionTask::ActionTask(Experimentation* experimentation, Led* L1, Led* L2, Pir* pir){
    this->experimentation = experimentation;
    this->L1 = L1;
    this->L2 = L2;
    this->pir = pir;
}

void ActionTask::init(int period){
    state = A0;
    tick();
    Task::init(period);
}

bool ActionTask::updateTimeAndCheckEvent(int basePeriod){
    bool result = false;
    State nextState = state;
    switch(state){
        case A0:
            if(this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION){
                nextState = A3;
                result = true;
                break;
            }
            if(this->experimentation->getExperimentationState() == Experimentation::State::SUSPENDED){
                nextState = A1;
                result = true;
                break;
            }
            if(this->experimentation->getExperimentationState() == Experimentation::State::ERROR){
                nextState = A2;
                result = true;
                break;
            }
            break;
        case A1:
        case A2:
        case A4:
            if(this->experimentation->getExperimentationState() == Experimentation::State::READY){
                nextState = A0;
                result = true;
                break;
            }
            break;
        case A3:
            if(this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION_CONCLUDED){
                nextState = A4;
                result = true;
                break;
            }
            break;
    }
    state = nextState;
    return result;
}

void ActionTask::tick(){
    switch(state){
        case A0:
            this->L1->switchOn();
            this->L2->switchOff();
            break;
        case A1:
            this->L1->switchOff();
            /* enabling interrupt to wake up arduino */
            this->pir->enableInterrupt();

            set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
            sleep_enable();
            sleep_mode();  
            /** The program will continue from here. **/  
            Serial.println("WAKE UP");
            /* First thing to do is disable sleep. */  
            sleep_disable();

            /* we disable the interrupt */
            this->pir->disableInterrupt();
        case A2:
            this->L1->switchOff();
            break;
        case A3:
            this->L1->switchOff();
            this->L2->switchOn();
            break;
        case A4:
            break;
    }
}