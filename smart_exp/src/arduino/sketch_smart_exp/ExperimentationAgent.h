#ifndef __EXPERIMENTATION_AGENT_H__
#define __EXPERIMENTATION_AGENT_H__

#include "Agent.h"
#include "Led.h"
#include "Button.h"
#include "Task.h"

class ExperimentationAgent : public Agent{
    Led* L2;
    Button* button;
    Task* task;
    int potPin;
    int myTime;
    int stateTimeElapsed;

    public:
        ExperimentationAgent(int buttonPin, int potPin);
        void init(int time);
        void setLeds(Led* L1, Led* L2);
        void setTask(Task* task);
        Task* getTask();
        State::STATE updateTimeAndCheckEvent(int baseTime);
    private:
        int roundToNeareastMultiple(int numToRound, int multiple);
};

#endif