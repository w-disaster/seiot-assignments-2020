#ifndef __ACTION_TASK__
#define __ACTION_TASK__

#include "Task.h"
#include "Led.h"
#include "Pir.h"

class ActionTask : public Task{
    Experimentation* experimentation;
    Led* L1;
    Led* L2;
    Pir* pir;
    enum State{A0, A1, A2, A3, A4};
    State state;

    public:
        ActionTask(Experimentation* experimentation, Pir* pir, Led* L1, Led* L2);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
    };

#endif

