#ifndef __BLINK_TASK__
#define __BLINK_TASK__

#include "Task.h"
#include "dam_remotehyd.h"

#define BL_TASK_T 200

/* This task lets the led blink when the river state is PRE ALARM */
class BlinkLedTaskClass : public Task {
   public:
        enum State {
            BL0,
            BL1,
            BL2
        };
        State state;

        BlinkLedTaskClass();
        void init(int period);
        void attachInt();
        void detachInt();
        static void tick();
    private:
        int period;
};

extern BlinkLedTaskClass* BlinkLedTask;

#endif
