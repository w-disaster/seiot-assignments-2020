#include "ServoMotorImpl.h"
#include "Arduino.h"

ServoMotorImpl::ServoMotorImpl(int pin){
  this->pin = pin;  
} 

void ServoMotorImpl::on(){
  motor.attach(pin);    
}

void ServoMotorImpl::setPosition(int angle){
  motor.write(MIN_SERVO + map(angle, 0, 180, 0, MAX_SERVO - MIN_SERVO));              
}

void ServoMotorImpl::off(){
  motor.detach();    
}