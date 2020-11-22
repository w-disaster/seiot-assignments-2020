#include "BlinkingTask.h"
#include "Arduino.h"

BlinkingTask::BlinkingTask(Led* led){
   this->led = led;
}

void BlinkingTask::init(int period){
    Task::init(period);
}

void BlinkingTask::tick(){
    if(this->ledState == HIGH){
        this->ledState = LOW;
        this->led->switchOff();
    } else {
        this->ledState = HIGH;
        this->led->switchOn();
    }
}