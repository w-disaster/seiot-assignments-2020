#include "Arduino.h"
#include "ControllerTask.h"

ControllerTaskClass* ControllerTask = new ControllerTaskClass();

ControllerTaskClass::ControllerTaskClass(){
    /* Set the initial state and led the msg to be sent */
    distance = Sonar->getDistance();
    state = getStateFromDistance(Sonar->getDistance());
    msgReady = true;
}

void ControllerTaskClass::init(int period){
    this->period = period;
}

void ControllerTaskClass::attachInt(){
    controllerTaskTicker.attach_ms(this->period, &ControllerTaskClass::tick);
}

void ControllerTaskClass::detachInt(){}

/* This method determine the state from the distance read */
ControllerTaskClass::State ControllerTaskClass::getStateFromDistance(float distance){
    ControllerTaskClass::State state;
    if(distance > d1_in_m){
        state = ControllerTaskClass::State::NORMAL;
    } else if(distance >= d2_in_m && distance <= d1_in_m){
        state = ControllerTaskClass::State::PRE_ALARM;
    } else {
        state = ControllerTaskClass::State::ALARM;
    }
    return state;
}

void ControllerTaskClass::tick(){
    distance = Sonar->getDistance();
    Serial.println("distance: " + String(distance));
    ControllerTaskClass::State nextState = getStateFromDistance(distance);

    switch(ControllerTask->state){
        case ControllerTaskClass::State::NORMAL:
            /* If the state is changed and it is:
                - PRE_ALARM: attach the blink task and the send task
                - ALARM: switch the led on and attach the send task
            */
            if(ControllerTask->state != nextState){
                msgReady = true;
                switch(nextState){
                    case ControllerTaskClass::State::PRE_ALARM:
                        BlinkLedTask->attachInt();
                        SendTask->init(SEND_TASK_PA_T);
                        break;
                    case ControllerTaskClass::State::ALARM:
                        Led->switchOn();
                        SendTask->init(SEND_TASK_A_T);
                        break;
                }
                SendTask->attachInt();
            }
            break;
        case ControllerTaskClass::State::PRE_ALARM:
            /* If the state is changed and it is:
                - NORMAL: detach the blink led and send tasks and switch off the led
                - ALARM: detach the blink led task, switch the led and 
                        change the period of the send task */
            if(ControllerTask->state != nextState){
                msgReady = true;
                BlinkLedTask->detachInt();
                switch(nextState){
                    case ControllerTaskClass::State::NORMAL:
                        Serial.println("prealarm -> normal");
                        SendTask->detachInt();
                        Led->switchOff();
                        break;
                    case ControllerTaskClass::State::ALARM:
                        Serial.println("prealarm -> alarm");
                        SendTask->init(SEND_TASK_A_T);
                        SendTask->attachInt();
                        Led->switchOn();
                        break;
                }
            }
            break;
        case ControllerTaskClass::State::ALARM:
             /* If the state is changed and it is:
                - PRE_ALARM: attach the blink led task, change the period of the send's
                - NORMAL: detach the send task, switch the led off */
            if(ControllerTask->state != nextState){
                msgReady = true;
                switch(nextState){
                    case ControllerTaskClass::State::PRE_ALARM:
                        Serial.println("alarm -> prealarm");
                        BlinkLedTask->attachInt();
                        SendTask->init(SEND_TASK_PA_T);
                        SendTask->attachInt();
                        break;
                    case ControllerTaskClass::State::NORMAL:
                        Serial.println("alarm -> normal");
                        SendTask->detachInt();
                        Led->switchOff();
                        break;
                }
            }
            break;
    }
    /* Update the state */
    ControllerTask->state = nextState;
}
