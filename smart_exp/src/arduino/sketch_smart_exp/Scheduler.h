#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Agent.h"
#include "State.h"
#include "Timer.h"

class Scheduler{
  private:
    virtual void setCurrentAgent(State::STATE state) = 0;
  public:
    virtual void init(int basePeriod) = 0;
    virtual void schedule() = 0;  
};
#endif
