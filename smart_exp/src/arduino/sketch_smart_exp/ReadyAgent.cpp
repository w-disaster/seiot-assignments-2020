#include "ReadyAgent.h"
#include "ButtonImpl.h"
#include "Arduino.h"

#define CALIBRATION_TIME 10

ReadyAgent::ReadyAgent(int buttonPin, int pirPin){
    this->button = new ButtonImpl(buttonPin);
    this->pirPin = pirPin;
    pinMode(this->pirPin, OUTPUT);
    task = NULL;

    Serial.print("Calibrating PIR ");
    for(int i = 0; i < CALIBRATION_TIME; i++){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
}

void ReadyAgent::init(int time){
    this->myTime = time;
    this->stateTimeElapsed = 0;
    this->L1->switchOn();
    this->L2->switchOff();
}   

void ReadyAgent::setLeds(Led* L1, Led* L2){
    this->L1 = L1;
    this->L2 = L2;
}

void ReadyAgent::setTask(Task* task){
    this->task = task;
}

Task* ReadyAgent::getTask(){
    return this->task;
}

State::STATE ReadyAgent::updateTimeAndCheckEvent(int baseTime){
    this->stateTimeElapsed += baseTime;
    if(this->stateTimeElapsed == myTime){
        this->stateTimeElapsed = 0;
        this->L1->switchOff();
        this->L2->switchOff();
        return State::STATE::SUSPENDED;
    }
    if(this->button->isPressed()){
        this->stateTimeElapsed = 0;
        this->L1->switchOff();
        this->L2->switchOff();
        if(digitalRead(pirPin) == HIGH){
            return State::STATE::EXPERIMENTATION;
        }
        return State::STATE::ERROR;
    }
    return State::STATE::READY;
}
