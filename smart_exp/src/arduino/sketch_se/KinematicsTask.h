#ifndef __KINEMATICS_TASK__
#define __KINEMATICS_TASK__

#include "Task.h"
#include "Sonar.h"
#include "ServoMotor.h"

#define MIN_FREQ 1
#define MAX_FREQ 25
#define MAX_VEL 15

#define POT_PIN A0 

class KinematicsTask : public Task{
    Experimentation* experimentation;
    Sonar* sonar;
    ServoMotor* servoMotor;
    enum State{K0, K1};
    State state;
    float precDistance, precSpeed;
    float maxSpeed;

    private:
        bool updateAndCheckTime(int basePeriod);
        int getPeriod();
        void init(int period);
        int roundToNearestMultiple(int numToRound, int multiple);
        float mapfloat(float value, float inMin, float inMax, float outMin, float outMax);

    public:
        KinematicsTask(Experimentation* experimentation, Sonar* sonar, int servoMotorPin);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif