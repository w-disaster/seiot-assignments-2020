#include "Scheduler.h"
#include "Arduino.h"

void Scheduler::init(int basePeriod){
    this->basePeriod = basePeriod;
    timer.setupPeriod(this->basePeriod);
    nTasks = 0;
}

bool Scheduler::addTask(Task* task){
  if (nTasks < MAX_TASKS-1){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false; 
  }
}
  
void Scheduler::schedule(){
  timer.waitForNextTick();
  //long t_start = micros();
  for (int i = 0; i < nTasks; i++){
    if (taskList[i]->updateTimeAndCheckEvent(basePeriod)){
      taskList[i]->tick();
    }
  }
  //Serial.println(String("tempo: ") + (micros() - t_start));
}