#ifndef __SERVO_MOTOR_TASK__
#define __SERVO_MOTOR_TASK__

#include "Task.h"
#include "RiverData.h"
#include "ServoMotor.h"

#define SERVO_MOTOR_PIN 12
#define MAX_DISTANCE_IN_M 1.0

class ServoMotorTask : public Task{

    RiverData* riverData;
    ServoMotor* servoMotor;

    enum State {
        S0,
        S1
    };
    State state;
    
    int lastDamOpening;

    public:
        ServoMotorTask(RiverData* riverData);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
    private:
        float mapfloat(float x, float in_min, float in_max, float outMin, float out_max);
};


#endif
