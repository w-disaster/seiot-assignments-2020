#ifndef __KINEMATICS_TASK_H__
#define __KINEMATICS_TASK_H__

#include "Task.h"
#include "ServoMotor.h"

class KinematicsTask : public Task{
    ServoMotor* servoMotor;
    Sonar* sonar;
        
    public:
        KinematicsTask(int trigPin, int echoPin, int DHTPin, int servoMotorPin);
        void init(int period);
        void tick();
}

#endif
