#ifndef __VIEWER_COM_TASK__
#define __VIEWER_COM_TASK__

#include "ViewerComunication.h"

#define DATA_SEPARATOR ":"
#define MSG_END ";"
#define STATE_END "!"
#define T 0
#define P 1
#define V 2
#define A 3

class ViewerComunicationTask : public ViewerComunication
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

private:
    void init(int period);
    void sendData(String msg, bool isState);

public:
    ViewerComunicationTask(Experimentation *experimentation);
    void getData(double[] data);
    bool updateTimeAndCheckEvent(int basePeriod);
    void tick();
    ~ViewerComunicationTask();
};

#endif