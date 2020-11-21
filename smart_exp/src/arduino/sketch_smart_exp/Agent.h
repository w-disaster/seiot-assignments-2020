#ifndef __AGENT_H__
#define __AGENT_H__

#include "Task.h"
#include "State.h"

class Agent{
    public:
        //int eventPin, Task* task
        virtual void init(int time) = 0;
        virtual void setTask(Task* task) = 0;
        virtual Task* getTask() = 0;
        virtual State::STATE updateTimeAndCheckEvent(int baseTime) = 0;
};

#endif
