#ifndef __TASK__
#define __TASK__

#include "ExperimentationStep.h"

class Task{
  int myPeriod;
  int timeElapsed;

  public:
    virtual void init(int period){
      myPeriod = period;
      timeElapsed = 0;
    }
    
    virtual bool updateTimeAndCheckEvent(int basePeriod) = 0;
    virtual void tick() = 0;

  protected:
    bool updateAndCheckTime(int basePeriod){
      timeElapsed += basePeriod;
      if(timeElapsed >= myPeriod){
        timeElapsed = 0;
        return true;
      }
      return false;
    }

    int getPeriod(){
      return myPeriod;
    }
};

#endif
