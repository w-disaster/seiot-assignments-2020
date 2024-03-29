#ifndef __VIEWER_COM_TASK__
#define __VIEWER_COM_TASK__

#include "Task.h"
#include "Arduino.h"
#include "KinematicsData.h"

#define DATA_SEPARATOR ":"
#define MSG_END ";"
#define STEP_END "!"
#define EXP_OVER '>'

class ViewerCommunicatorTask : public Task {

    enum State { VC0, VC1, VC2 };
    /* fields */
    State state;
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
    ViewerCommunicatorTask(ExperimentationStep *experimentationStep, KinematicsData *kinematicsData);
    bool updateTimeAndCheckEvent(int basePeriod);
    void tick();
};

#endif
