#ifndef __KINEMATICS_TASK__
#define __KINEMATICS_TASK__

#include "Task.h"
#include "Sonar.h"
#include "ServoMotor.h"

#define POT_PIN A0 

class KinematicsTask : public Task{
    Experimentation* experimentation;
    Sonar* sonar;
    ServoMotor* servoMotor;
    enum State{K0, K1};
    State state;
    float precDistance, precSpeed;
    /*int i;
    float avg_vel[1000];*/

    private:
        bool updateAndCheckTime(int basePeriod);
        int getPeriod();
        void init(int period);
        int roundToNearestMultiple(int numToRound, int multiple);

    public:
        KinematicsTask(Experimentation* experimentation, int trigPin, int echoPinf, int servoMotorPin);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif