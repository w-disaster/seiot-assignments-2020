#include "SchedulerImpl.h"
#include "Arduino.h"

SchedulerImpl::SchedulerImpl()
{
  this->nTask = 0;
}

void SchedulerImpl::addTask(Task *task)
{
  this->tasks[nTask] = task;
  nTask++;
}

void SchedulerImpl::schedule()
{
}
