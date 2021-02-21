#include "ServoMotorTask.h"
#include "ServoMotorImpl.h"
#include "Arduino.h"

ServoMotorTask::ServoMotorTask(RiverData* riverData){
    this->riverData = riverData;
    this->lastDamOpening = riverData->getDamOpening();
    this->servoMotor = new ServoMotorImpl(SERVO_MOTOR_PIN);
}

void ServoMotorTask::init(int period){
    Task::init(period);
    this->state = S0;
    this->servoMotor->on();
}

bool ServoMotorTask::updateTimeAndCheckEvent(int basePeriod){
    if(Task::updateAndCheckTime(basePeriod)){
        int damOpening = this->riverData->getDamOpening();
        
        /* If the distance is different from the previous, new one has been sampled
         *  then we change State to SM1 to move the servo motor
         */
        if(this->lastDamOpening != damOpening){
            this->lastDamOpening = damOpening;
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
        case S0:
            Serial.println("setting to 0");
            this->servoMotor->setPosition(0);
            break;
        case S1:
            /* We calculate the value in grades in order to write it */ 
            int angle = mapfloat(this->lastDamOpening, 0, 100, 0, 180);
            Serial.println(String("setting to: ") + angle);
            this->servoMotor->setPosition(angle);
            break;
    }
}

float ServoMotorTask::mapfloat(float x, float inMin, float inMax, float outMin, float outMax) {
 return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
