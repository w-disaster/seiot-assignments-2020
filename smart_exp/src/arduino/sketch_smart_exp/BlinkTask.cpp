#include "BlinkTask.h"
#include "Arduino.h"

BlinkTask::BlinkTask(Led* led){
   this->led = led;
}

void BlinkTask::init(int period){
    Task::init(period);
}

void BlinkTask::tick(){
    if(this->ledState == HIGH){
        this->ledState = LOW;
        this->led->switchOff();
    } else {
        this->ledState = HIGH;
        this->led->switchOn();
    }
}