#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Task.h"

class Scheduler
{

public:
  /**
   * Adds a Task to the list of tasks to schedule.
   */
  virtual void addTask(Task *task) = 0;

  /**
   * Goes to the next Task.
   */
  virtual void schedule() = 0;
};
#endif
