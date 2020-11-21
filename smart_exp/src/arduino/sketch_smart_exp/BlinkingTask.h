#ifndef __BLINKING_TASK_H__
#define __BLINKING_TASK_H__

#include "Task.h"

class BlinkingTask : public Task{
    int pin;
    int ledState;

    public:
        BlinkingTask(int pin);
        void init(int period);
        void tick();
};

#endif