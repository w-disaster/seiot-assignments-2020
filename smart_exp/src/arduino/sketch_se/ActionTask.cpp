#include "ActionTask.h"
#include <Arduino.h>
#include <avr/sleep.h>

ActionTask::ActionTask(ExperimentationStep* experimentationStep, Pir* pir, Led* L1, Led* L2){
    this->experimentationStep = experimentationStep;
    this->pir = pir;
    this->L1 = L1;
    this->L2 = L2;
}

void ActionTask::init(int period){
    state = A0;
    tick();
    Task::init(period);
}

bool ActionTask::updateTimeAndCheckEvent(int basePeriod){
    bool result = false;
    State nextState = state;
    ExperimentationStep::Step expStep = this->experimentationStep->getStep();
    switch(state){
        case A0:
            if(updateAndCheckTime(basePeriod)){
                if(expStep == ExperimentationStep::Step::EXPERIMENTATION){
                    nextState = A3;
                    result = true;
                    break;
                }
                if(expStep == ExperimentationStep::Step::SUSPENDED){
                    nextState = A1;
                    result = true;
                    break;
                }
                if(expStep == ExperimentationStep::Step::ERROR){
                    nextState = A2;
                    result = true;
                    break;
                }
            }
            break;
        case A1:
        case A2:
        case A4:
            if(updateAndCheckTime(basePeriod)){
                if(expStep == ExperimentationStep::Step::READY){
                    nextState = A0;
                    result = true;
                    break;
                }
            }
            break;
        case A3:
            if(updateAndCheckTime(basePeriod)){
                if(expStep == ExperimentationStep::Step::EXPERIMENTATION_CONCLUDED){
                    nextState = A4;
                    result = true;
                    break;
                }
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
            this->pir->interrupt();

            set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
            sleep_enable();
            sleep_mode();  
            /** The program will continue from here. **/  
            
            /* First thing to do is disable sleep. */  
            sleep_disable();

            /* we disable the interrupt */
            this->pir->noInterrupt();
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