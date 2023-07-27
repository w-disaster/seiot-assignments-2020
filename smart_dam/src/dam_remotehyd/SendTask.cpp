#include "Arduino.h"
#include "SendTask.h"

SendTaskClass* SendTask = new SendTaskClass();

SendTaskClass::SendTaskClass(){
    state = S0;
    msgReady = false;
}

void SendTaskClass::init(int period){
    this->period = period;
}

void SendTaskClass::attachInt(){
    state = S1;
    sendTaskTicker.attach_ms(this->period, &SendTaskClass::tick);
}

void SendTaskClass::detachInt(){
    state = S0;
    sendTaskTicker.detach();
}

void SendTaskClass::tick(){
    switch(SendTask->state){
        case SendTaskClass::State::S0:
            break;
        case SendTaskClass::State::S1:
            msgReady = true;
            break;
    }
}
