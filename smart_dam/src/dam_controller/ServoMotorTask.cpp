#include "ServoMotorTask.h"
#include "ServoMotorImpl.h"
#include "Arduino.h"

ServoMotorTask::ServoMotorTask(RiverData* riverData){
    this->riverData = riverData;
    this->lastDistance = riverData->getDistance();
    this->servoMotor = new ServoMotorImpl(SERVO_MOTOR_PIN);
}

void ServoMotorTask::init(int period){
    Task::init(period);
    this->state = SM0;
    this->servoMotor->on();
}

bool ServoMotorTask::updateTimeAndCheckEvent(int basePeriod){
    if(Task::updateAndCheckTime(basePeriod)){
        float distance = this->riverData->getDistance();
        if(this->lastDistance != distance){
            this->lastDistance = distance;
            this->state = SM1;
            return true;
        }
        this->state = SM0;
    }
    return false;
}

void ServoMotorTask::tick(){
    switch(this->state){
        case SM0:
            break;
        case SM1:
            int angle = map(this->distance, 0, MAX_DISTANCE_IN_M, 0, 180);
            this->servoMotor->setPosition(angle);
            break;
    }
}
