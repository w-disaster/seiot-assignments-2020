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
    
    float lastDistance;

    enum State {
        SM0,
        SM1
    };
    State state;
    
    public:
        ServoMotorTask(RiverData* riverData);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};


#endif
