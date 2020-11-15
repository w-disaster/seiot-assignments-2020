#ifndef __SCHEDULER_IMPL__
#define __SCHEDULER_IMPL__

#include "Scheduler.h"

//TODO: Update value
#define N_TASKS 10

class SchedulerImpl: public Scheduler
{
public:
  SchedulerImpl();
  void addTask(Task* task);
  void removeTask(Task* task);
private:
  Task* tasks[N_TASKS];
};

#endif
