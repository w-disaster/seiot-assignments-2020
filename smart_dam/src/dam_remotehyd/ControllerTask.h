#ifndef __CONTROLLER_TASK__
#define __CONTROLLER_TASK__

#include "Task.h"
#include "dam_remotehyd.h"

#define CTR_TASK_T 1000

class ControllerTaskClass : public Task {
    public:
        enum State {
            NORMAL,
            PRE_ALARM,
            ALARM
        };
        State state;

        ControllerTaskClass();
        void init(int period);
        void attachInt();
        void detachInt();
        static State getStateFromDistance(float distance);
        static void tick();
    private:
        int period;
};

extern ControllerTaskClass* ControllerTask;

#endif