#ifndef __VIEWER_COM_TASK__
#define __VIEWER_COM_TASK__

#include "Task.h"
#include "Arduino.h"
#include "KinematicsData.h"

#define DATA_SEPARATOR ":"
#define MSG_END ";"
#define STATE_END "!"
#define EXP_OVER '>'

class ViewerComunicationTask : public Task
{

    enum State{ VC0, VC1, VC2 };
    /* fields */
    State state;
    Experimentation::State currentExpState;
    Experimentation *experimentation;
    KinematicsData *kinematicsData;
    bool stateMsgAlreadySent;
    long expRelativeTime;

private:
    /* functions */
    void init(int period);
    void sendData(String msg, bool isState);
    void sendStateMsgOnce(String stateKey);
    void sendExperimentData(String data);
    String format(float t, float p, float v, float a);

public:
    ViewerComunicationTask(Experimentation *experimentation, KinematicsData *kinematicsData);
    bool updateTimeAndCheckEvent(int basePeriod);
    void tick();
    ~ViewerComunicationTask();
};

#endif
