#ifndef __KINEMATICS_TASK__
#define __KINEMATICS_TASK__

#include "Task.h"
#include "Sonar.h"
#include "ServoMotor.h"
#include "lib.h"

#define MIN_FREQ 1.0
#define MAX_FREQ 10.0
#define MIN_PERIOD_MS 1/MAX_FREQ * SEC_TO_MS
#define MAX_PERIOD_MS 1/MIN_FREQ * SEC_TO_MS
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
        void init(int period);
        int roundToNearestMultiple(int numToRound, int multiple);
        float mapfloat(float value, float inMin, float inMax, float outMin, float outMax);

    public:
        KinematicsTask(Experimentation* experimentation, Sonar* sonar, int servoMotorPin);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif