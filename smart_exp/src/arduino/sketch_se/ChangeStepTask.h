#ifndef __CHANGE_STEP_TASK__
#define __CHANGE_STEP_TASK__

#include "Task.h"
#include "Button.h"
#include "Sonar.h"

#define SLEEP_TIME 5
#define MAX_TIME 20
#define ERROR_TIME 2
#define MILLIS_TO_SEC 1000

class ChangeStepTask : public Task{
    ExperimentationStep* experimentationStep;
    Sonar* sonar;
    Button* bStart;
    Button* bStop;
    
    enum Step{ES0, ES1, ES2, ES3, ES4};
    Step step;

    public:
        ChangeStepTask(ExperimentationStep* experimentationStep, Sonar* sonar, int bStartPin, int bStopPin);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();  
};

#endif