#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Agent.h"
#include "State.h"

class Scheduler{
  public:
    virtual void schedule() = 0;  
    virtual void setState(State::STATE state) = 0;
};
#endif
