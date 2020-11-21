#ifndef __SCHEDULER_IMPL_H__
#define __SCHEDULER_IMPL_H__

#include "Scheduler.h"
#define NUM_STATES 5

class SchedulerImpl : public Scheduler{
  State::STATE state;
  public:
    SchedulerImpl();
    void schedule();
    void setState(State::STATE state);
};

#endif
