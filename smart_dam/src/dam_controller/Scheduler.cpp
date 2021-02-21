#include "Scheduler.h"
#include "Arduino.h"

void Scheduler::init(int basePeriod){
  this->basePeriod = basePeriod;
  timer.setupPeriod(this->basePeriod);
  nTasks = 0;
}

bool Scheduler::addTask(Task *task){
  if (nTasks < MAX_TASKS){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else{
    return false;
  }
}

void Scheduler::schedule(){
  timer.waitForNextTick();
  for (int i = 0; i < nTasks; i++){
    if (taskList[i]->updateTimeAndCheckEvent(basePeriod)){
      taskList[i]->tick();
    }
  }
}
