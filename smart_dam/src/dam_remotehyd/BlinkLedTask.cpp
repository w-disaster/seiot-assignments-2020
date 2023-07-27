#include "Arduino.h"
#include "BlinkLedTask.h"

BlinkLedTaskClass* BlinkLedTask = new BlinkLedTaskClass();

BlinkLedTaskClass::BlinkLedTaskClass(){
    this->state = BL0;
}

void BlinkLedTaskClass::init(int period){
    this->period = period;
}

void BlinkLedTaskClass::attachInt(){
    this->state = BL1;
    blinkLedTaskTicker.attach_ms(this->period, &BlinkLedTaskClass::tick);
}

void BlinkLedTaskClass::detachInt(){
    this->state = BL0;
    blinkLedTaskTicker.detach();
}

void BlinkLedTaskClass::tick(){
    switch(BlinkLedTask->state){
        case BlinkLedTaskClass::State::BL0:
            break;
        case BlinkLedTaskClass::State::BL1:
            Led->switchOn();
            BlinkLedTask->state = BlinkLedTaskClass::State::BL2;
            break;
        case BlinkLedTaskClass::State::BL2:
            Led->switchOff();
            BlinkLedTask->state = BlinkLedTaskClass::State::BL1;
            break;
    }
}
