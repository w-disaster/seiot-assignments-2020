#include "ReadyAgent.h"
#include "Arduino.h"

ReadyAgent::ReadyAgent(int buttonPin, int pirPin){
    this->buttonPin = buttonPin;
    this->pirPin = pirPin;
    this->task = NULL;
}

void ReadyAgent::init(int time){
    this->myTime = time;
    this->stateTimeElapsed = 0;
}   

void ReadyAgent::setTask(BlinkingTask* task){
    this->task = task;
}

BlinkingTask* ReadyAgent::getTask(){
    return this->task;
}

State::STATE ReadyAgent::updateTimeAndCheckEvent(int baseTime){
    this->stateTimeElapsed += baseTime;
    if(this->stateTimeElapsed == myTime){
        this->timeElapsed = 0;
        return State::STATE::SUSPENDED;
    }
    if(digitalRead(this->buttonPin) == HIGH){
        /*if(digitalRead(pirPin) == HIGH){
            return State::STATE::EXPERIMENTATION;
        }*/
        return State::STATE::ERROR;
    }
}
