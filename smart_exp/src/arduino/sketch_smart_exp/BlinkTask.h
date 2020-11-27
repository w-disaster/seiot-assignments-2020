#ifndef __BLINKING_TASK_H__
#define __BLINKING_TASK_H__

#include "Task.h"
#include "Led.h"

class BlinkTask : public Task{
    Led* led;
    int ledState;

    public:
        BlinkTask(Led* led);
        void init(int period);
        void tick();
};

#endif