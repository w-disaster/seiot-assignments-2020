#ifndef __KINEMATICS_TASK__
#define __KINEMATICS_TASK__

#include "Task.h"
#include "Sonar.h"

#define POT_PIN A0 

class KinematicsTask : public Task{
    Experimentation* experimentation;
    Sonar* sonar;
    enum {K0, K1} state, prec;

    private:
        bool updateAndCheckTime(int basePeriod);
        void init(int period);
        int roundToNearestMultiple(int numToRound, int multiple);

    public:
        KinematicsTask(Experimentation* experimentation, int trigPin, int echoPin);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif