#ifndef __SCHEDULER_IMPL_H__
#define __SCHEDULER_IMPL_H__

#include "Scheduler.h"
#include "Agent.h"
#define NUM_STATES 5

class SchedulerImpl : public Scheduler{
  State::STATE state;
  Timer timer;
  Agent* agents[NUM_STATES];
  Agent* current;
  int basePeriod;
  
  private:
    void setCurrentAgent(State::STATE state);

  public:
    SchedulerImpl();
    void init(int basePeriod);
    void schedule();
};

#endif
