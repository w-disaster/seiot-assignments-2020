#ifndef __READY_AGENT_H__
#define __READY_AGENT_H__

#include "Agent.h"
//#include "BlinkingTask.h"
#include "Button.h"
#define CALIBRATION_TIME 10

class ReadyAgent : public Agent{
    private:
      Task* task;
      Button* button;
      Led* L1;
      Led* L2;
      int pirPin;
      int myTime;
      int stateTimeElapsed;
      
    public:
        ReadyAgent(int buttonPin, int pirPin);
        void init(int time);
        void setLeds(Led* L1, Led* L2);
        void setTask(Task* task);
        Task* getTask();
        State::STATE updateTimeAndCheckEvent(int baseTime);
};

#endif
