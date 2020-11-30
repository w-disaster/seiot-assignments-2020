#ifndef __BLINK_LED_TASK__
#define __BLINK_LED_TASK__

#include "Task.h"
#include "Led.h"

class BlinkLedTask : public Task{
    Experimentation* experimentation;
    Led* led;
    enum State{BL0, BL1, BL2};
    State state;

    public:
        BlinkLedTask(Experimentation* experimentation, Led* led);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};

#endif