#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Task.h"

class Scheduler
{

public:
  /**
   * Adds a Task to the list of tasks to schedule.
   */
  virtual void addTask(Task* task) = 0;
  
  /**
   * Removes a Task from the list of tasks to schedule.
   */
  virtual void removeTask(Task* task) = 0;
};
#endif
