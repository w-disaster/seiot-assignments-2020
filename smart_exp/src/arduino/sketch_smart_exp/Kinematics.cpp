#include "KinematicsTask.h"
#include "Arduino.h"

#include "ServoMotorImpl.h"

KinematicsTask::KinematicsTask(int trigPin, int echoPin, int DHTPin, int servoMotorPin){
    this->servoMotor = new ServoMotorImpl(servoMotorPin);
    this->servoMotor->on();

}

KinematicsTask::void init(int period){
}

KinematicsTask::void tick(){

}