#ifndef __AGENT_H__
#define __AGENT_H__

#include "Task.h"
#include "State.h"
#include "Led.h"

class Agent{
    public:
        virtual void init(int time) = 0;
        virtual void setLeds(Led* L1, Led* L2) = 0;
        virtual void setTask(Task* task) = 0;
        virtual Task* getTask() = 0;
        virtual State::STATE updateTimeAndCheckEvent(int baseTime) = 0;
};

#endif
