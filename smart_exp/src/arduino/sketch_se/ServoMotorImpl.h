#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include "ServoMotor.h"
#include <Arduino.h>
#include "ServoTimer2.h"

#define MAX_SERVO 2250
#define MIN_SERVO 750

class ServoMotorImpl: public ServoMotor {

public:
  ServoMotorImpl(int pin);

  void on();
  void setPosition(int angle);
  void off();
    
private:
  int pin; 
  ServoTimer2 motor; 
};

#endif