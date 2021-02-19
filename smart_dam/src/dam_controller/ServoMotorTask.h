#ifndef __SERVO_MOTOR_TASK__
#define __SERVO_MOTOR_TASK__

#include "Task.h"
#include "RiverData.h"
#include "ServoMotor.h"

#define SERVO_MOTOR_PIN 7
#define MAX_DISTANCE_IN_M 5.0

class ServoMotorTask : public Task{

    RiverData* riverData;
    ServoMotor* servoMotor;

    enum State {
        S0,
        S1
    };
    State state;
    
    float lastDistance;

    public:
        ServoMotorTask(RiverData* riverData);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};


#endif
