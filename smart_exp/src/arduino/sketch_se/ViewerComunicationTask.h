#ifndef __VIEWER_COM_TASK__
#define __VIEWER_COM_TASK__

#include "Task.h"
#include "Arduino.h"
#include "KinematicsData.h"

#define DATA_SEPARATOR ":"
#define MSG_END ";"
#define STEP_END "!"
#define EXP_OVER '>'

class ViewerComunicationTask : public Task {

    enum Step { 
        VC0, 
        VC1, 
        VC2 
    };
    /* fields */
    Step step;
    ExperimentationStep::Step currentExpStep;
    ExperimentationStep *experimentationStep;
    KinematicsData *kinematicsData;
    bool stepMsgAlreadySent;
    long expRelativeTime;

private:
    /* functions */
    void init(int period);
    void sendData(String msg, bool isStep);
    void sendStepMsgOnce(String stepKey);
    void sendExperimentData(String data);
    String format(float t, float p, float v, float a);

public:
    ViewerComunicationTask(ExperimentationStep *experimentationStep, KinematicsData *kinematicsData);
    bool updateTimeAndCheckEvent(int basePeriod);
    void tick();
    ~ViewerComunicationTask();
};

#endif
