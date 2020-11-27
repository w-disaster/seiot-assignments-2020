#include "ExperimentationAgent.h"
#include "Arduino.h"

#include "ButtonImpl.h"

ExperimentationAgent::ExperimentationAgent(int buttonPin, int potPin){
    this->button = new ButtonImpl(buttonPin);
    this->potPin = potPin;
    this->task = NULL;
    this->L2 = NULL;
}

void ExperimentationAgent::init(int time){
    //try, catch exception?
    this->myTime = time;
    this->stateTimeElapsed = 0;
    this->L2->switchOn();
    getTask()->init(roundToNearestMultiple(1050 - map(analogRead(this->potPin), 0, 1023, 50, 1000)));
}

void ExperimentationAgent::setLeds(Led* L1, Led* L2){
    this->L2 = L2;
}

void ExperimentationAgent::setTask(Task* task){
    this->task = task;
}

Task* ExperimentationAgent::getTask(){
    return this->task;
}

State::STATE ExperimentationAgent::updateTimeAndCheckEvent(int baseTime){
    this->stateTimeElapsed += baseTime;
    if(this->stateTimeElapsed == myTime || this->button->isPressed()){
        this->stateTimeElapsed = 0;
        this->L2->switchOff();
        return State::STATE::EXPERIMENTATION_CONCLUDED;
    }
    return State::STATE::EXPERIMENTATION;
}

int ExperimentationAgent::roundToNeareastMultiple(int numToRound, int multiple){
    int remainder = numToRound % multiple;
    if(multiple == 0 || remainder == 0){
        return numToRound;
    }
    if(numToRound % multiple < multiple / 2){
        return numToRound - remainder;
    }
    return numToRound + multiple - remainder;
}