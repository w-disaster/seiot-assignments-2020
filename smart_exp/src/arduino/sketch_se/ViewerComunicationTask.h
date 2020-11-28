#ifndef __VIEWER_COM_TASK__
#define __VIEWER_COM_TASK__

#include "Task.h"
#include "Arduino.h"

#define DATA_SEPARATOR ":"
#define MSG_END ";"
#define STATE_END "!"

class ViewerComunicationTask : public Task
{

    enum State
    {
        VC0,
        VC1,
        VC2,
        VC3,
        VC4
    } state;
    Experimentation *experimentation;
    bool stateMsgAlreadySent;
    long expRelativeTime;

private:
    void init(int period);
    void sendData(String msg, bool isState);

public:
    ViewerComunicationTask(Experimentation *experimentation);
    bool updateTimeAndCheckEvent(int basePeriod);
    void tick();
    ~ViewerComunicationTask();
};

#endif