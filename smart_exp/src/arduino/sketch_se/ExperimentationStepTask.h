#ifndef __EXPERIMENTATION_STEP_TASK__
#define __EXPERIMENTATION_STEP_TASK__

#include "Task.h"
#include "Button.h"

#define SLEEP_TIME 5
#define MAX_TIME 20
#define ERROR_TIME 2
#define MILLIS_TO_SEC 1000

class ExperimentationStepTask : public Task{
    Experimentation* experimentation;
    Button* bStart;
    Button* bStop;
    int pirPin;
    
    enum State{ES0, ES1, ES2, ES3, ES4};
    State state;

    private:
        bool updateAndCheckTime(int basePeriod);

    public:
        ExperimentationStepTask(Experimentation* experimentation, int bStartPin, int bStopPin, int pir);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif