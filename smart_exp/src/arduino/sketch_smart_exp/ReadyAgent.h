#ifndef __READY_AGENT_H__
#define __READY_AGENT_H__

#include "Agent.h"
#include "BlinkingTask.h"

class ReadyAgent : Agent{
    private:
      BlinkingTask* task;
      int buttonPin;
      int pirPin;
      int myTime;
      int stateTimeElapsed;
      
    public:
        ReadyAgent(int buttonPin, int pirPin);
        void init(int time);
        void setTask(BlinkingTask* task);
        BlinkingTask* getTask();
        State::STATE updateTimeAndCheckEvent(int baseTime);
};

#endif
