#ifndef __SEND_TASK__
#define __SEND_TASK__

#include "Task.h"
#include "dam_remotehyd.h"

#define SEND_TASK_PA_T 10 * 1000
#define SEND_TASK_A_T 5 * 1000

/* This task sets the message ready to be sent to Dam Service:
    it only assigns msgReady = true and let the loop() do HTTP POST.
    In fact, since the tick is an ISR, we don't execute activities that may take
    too time. */
class SendTaskClass : public Task {
    public:
        enum State {
            S0,
            S1
        };
        State state;

        SendTaskClass();
        void init(int period);
        void attachInt();
        void detachInt();
        static void tick();
    private:
        int period;
};

extern SendTaskClass* SendTask;

#endif
