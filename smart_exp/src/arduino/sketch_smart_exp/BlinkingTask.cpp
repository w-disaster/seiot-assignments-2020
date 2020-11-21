#include "BlinkingTask.h"
#include "Arduino.h"

BlinkingTask::BlinkingTask(int pin){
   this->pin = pin;
}

void BlinkingTask::init(int period){
    Task::init(period);
}

void BlinkingTask::tick(){
    this->ledState = this->ledState == HIGH ? LOW : HIGH;
    digitalWrite(this->pin, this->ledState);    
}