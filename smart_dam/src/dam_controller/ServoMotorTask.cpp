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
    this->state = S0;
    this->servoMotor->on();
}

bool ServoMotorTask::updateTimeAndCheckEvent(int basePeriod){
    if(Task::updateAndCheckTime(basePeriod)){
        float distance = this->riverData->getDistance();
        
        /* If the distance is different from the previous, new one has been sampled
         *  then we change State to SM1 to move the servo motor
         */
        if(this->lastDistance != distance){
            Serial.println(distance);
            this->lastDistance = distance;
            this->state = S1;
            return true;
        }
        /* In other cases we can move the servo motor anyway but we don't, in 
         *  order not to overload it of writes
         */
        this->state = S0;
    }
    return false;
}

void ServoMotorTask::tick(){
    switch(this->state){
        case SM0:
            break;
        case SM1:
            /* We calculate the value in grades in order to write it */ 
            int angle = map(this->lastDistance, 0, MAX_DISTANCE_IN_M, 0, 180);
            this->servoMotor->setPosition(angle);
            break;
    }
}
