#include "Arduino.h"
#include "SonarImpl.h"
#include "ServoMotorImpl.h"
#include "KinematicsTask.h"

KinematicsTask::KinematicsTask(Experimentation* experimentation, Sonar* sonar, int servoMotorPin){
    this->experimentation = experimentation;
    this->sonar = sonar;
    this->servoMotor = new ServoMotorImpl(servoMotorPin);
    
    this->state = K0;
    pinMode(POT_PIN, INPUT);
}

bool KinematicsTask::updateTimeAndCheckEvent(int basePeriod){
    State nextState = state;
    bool result;
    switch(state){
        case K0:
            if(this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION){
                /* sampling rate */
                int potValue = analogRead(POT_PIN);

                //int period = roundToNearestMultiple(1020 - map(potValue, 0, 1023, 1/MAX_FREQ * 1000, 1/MIN_FREQ * 1000), 20);
                int period = roundToNearestMultiple(1050 - map(potValue, 0, 1023, 50, 1000), 50);
                init(period);
                Serial.println(String("periodo task cinematica: ") + period + " pot value: " + potValue);

                /* sound speed update */
                this->sonar->updateSoundSpeed();
                /* servo motor on */
                this->servoMotor->on();
                this->servoMotor->setPosition(0);
                /* start calculating speed from point x0 = 0 and acceleration from v0 = 0*/
                precDistance = 0;
                precSpeed = 0;
                maxSpeed = (MAX_DISTANCE - MIN_DISTANCE) / (period * 0.001);
                Serial.println(maxSpeed);
                nextState = K1;
            }
            result = true;
            break;
        case K1:
            if(updateAndCheckTime(basePeriod)){
                if(this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION){
                    result = true;
                    break;
                } else {
                    nextState = K0;
                    this->servoMotor->setPosition(0);
                    this->servoMotor->off();
                    result = false;
                    break;
                }
            }
            if(this->experimentation->getExperimentationState() != Experimentation::State::EXPERIMENTATION){
                nextState = K0;
                this->servoMotor->setPosition(0);
                this->servoMotor->off();
            }
            result = false;
            break;
    }
    state = nextState;
    return result;
}

void KinematicsTask::tick(){
    switch(state){
        case K0:
            break;
        case K1:
            float distance = this->sonar->getDistance();
            float deltat = getPeriod() * 0.001;
            float speed = (distance - precDistance) / deltat;

            if(abs(speed) > maxSpeed){
                speed = 0;
            }
            float acceleration = (speed - precSpeed) / deltat;

            int value = mapfloat(abs(speed), 0, maxSpeed, 0, 180);
            Serial.println(distance);
            Serial.println(speed);
            Serial.println(value);

            Serial.println("----------------------");
            
            this->servoMotor->setPosition(value);

            /* write to communicator */
            this->experimentation->setDistance(distance);
            this->experimentation->setSpeed(speed);
            this->experimentation->setAcceleration(acceleration);

            /* prec values update */
            precDistance = distance;
            precSpeed = speed;
            break;
    }
}

float KinematicsTask::mapfloat(float value, float inMin, float inMax, float outMin, float outMax)
{
 return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void KinematicsTask::init(int period){
    Task::init(period);
}

bool KinematicsTask::updateAndCheckTime(int basePeriod){
    return Task::updateAndCheckTime(basePeriod);
}

int KinematicsTask::getPeriod(){
    return Task::getPeriod();
}

int KinematicsTask::roundToNearestMultiple(int numToRound, int multiple){
    int remainder = numToRound % multiple;
    if(multiple == 0 || remainder == 0){
        return numToRound;
    }
    if(numToRound % multiple < multiple / 2){
        return numToRound - remainder;
    }
    return numToRound + multiple - remainder;
}