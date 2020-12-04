#ifndef __STEP_TASK__
#define __STEP_TASK__

#include "Task.h"
#include "Button.h"
#include "Sonar.h"

#define SLEEP_TIME 5
#define MAX_TIME 20
#define ERROR_TIME 2
#define MILLIS_TO_SEC 1000

class StepTask : public Task{
    Experimentation* experimentation;
    Sonar* sonar;
    Button* bStart;
    Button* bStop;
    
    enum State{ES0, ES1, ES2, ES3, ES4};
    State state;

    public:
        StepTask(Experimentation* experimentation, Sonar* sonar, int bStartPin, int bStopPin);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif