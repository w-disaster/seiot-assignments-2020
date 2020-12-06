#ifndef __ACTION_TASK__
#define __ACTION_TASK__

#include "Task.h"
#include "Led.h"
#include "Pir.h"

class ActionTask : public Task{
    ExperimentationStep* experimentationStep;
    Led* L1;
    Led* L2;
    Pir* pir;
    enum Step{A0, A1, A2, A3, A4};
    Step step;

    public:
        ActionTask(ExperimentationStep* experimentationStep, Pir* pir, Led* L1, Led* L2);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
    };

#endif

