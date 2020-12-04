#include "ViewerComunicationTask.h"
#include "Arduino.h"

ViewerComunicationTask::ViewerComunicationTask(Experimentation *experimentation, KinematicsData *kinematicsData)
{
    this->experimentation = experimentation;
    this->kinematicsData = kinematicsData;
    this->stateMsgAlreadySent = false;
}

void ViewerComunicationTask::sendData(String msg, bool isState)
{
    isState ? Serial.print(msg + STATE_END + MSG_END) : Serial.print(msg + MSG_END);
}

void ViewerComunicationTask::sendStateMsgOnce(String stateKey)
{
    if (!stateMsgAlreadySent)
    {
        /* sends the State to serial */
        sendData(stateKey, true);

        if (state == VC3)
        {
            /* start the relative clock if we are experimenting */
            expRelativeTime = micros();
        }
        stateMsgAlreadySent = true;
    }
}

void ViewerComunicationTask::sendExperimentData(String data)
{
    /* sends msg ad experiment data */
    sendData(data, false);
    this->kinematicsData->setDataReady(false);
}

String ViewerComunicationTask::format(float t, float p, float v, float a)
{
    String formattedString = String(t) + DATA_SEPARATOR + String(p) + DATA_SEPARATOR + String(v) + DATA_SEPARATOR + String(a);
    return formattedString;
}

void ViewerComunicationTask::init(int period)
{
    this->state = VC0;
    Task::init(period);
}

bool ViewerComunicationTask::updateTimeAndCheckEvent(int basePeriod)
{
    bool result = false;
    State nextState = state;
    Experimentation::State expState = this->experimentation->getExperimentationState();

    switch (state)
    {
    case VC0:
        /* ready */
        if (expState == Experimentation::State::SUSPENDED)
        {
            /* goes to suspended */
            nextState = VC1;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        else if (expState == Experimentation::State::ERROR)
        {
            /* goes to error */
            nextState = VC2;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        else if (expState == Experimentation::State::EXPERIMENTATION)
        {
            /* goes to exp */
            nextState = VC3;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;

    case VC1:
        /* suspended */
        if (expState == Experimentation::State::READY)
        {
            /* goes to ready */
            nextState = VC0;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;

    case VC2:
        /* error */
        if (expState == Experimentation::State::READY)
        {
            /* goes to ready */
            nextState = VC0;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;

    case VC3:
        /* exp */
        if (expState == Experimentation::State::EXPERIMENTATION_CONCLUDED)
        {
            /* goes to exp over */
            nextState = VC4;
            this->stateMsgAlreadySent = false;
        }
        /* keep ticking */
        result = true;
        break;

    case VC4:
        /* exp over */
        if (expState == Experimentation::State::READY)
        {
            /* goes to ready */
            nextState = VC0;
            this->stateMsgAlreadySent = false;
        }
        /* keep ticking */
        result = true;
        break;
    }
    state = nextState;
    return result;
}

void ViewerComunicationTask::tick()
{
    switch (state)
    {
    case VC0:
        /* ready */
        sendStateMsgOnce("ready");
        delay(5);
        break;
    case VC1:
        /* suspended */
        sendStateMsgOnce("suspended");
        delay(5);
        break;
    case VC2:
        /* error */
        sendStateMsgOnce("error");
        break;
    case VC3:
        /* exp */
        sendStateMsgOnce("exp");

        if (this->kinematicsData->isDataReady())
        {
            sendExperimentData(format((micros() - this->expRelativeTime) / 1000,
                                      this->kinematicsData->getDistance(),
                                      this->kinematicsData->getSpeed(),
                                      this->kinematicsData->getAcceleration()));
        }
        break;
    case VC4:
        /* exp over */
        sendStateMsgOnce("over");
        /* every tick checks if the user pressed ok */
        if ((char)Serial.read() == EXP_OVER)
        {
            /* tells experimentation to change State */
            this->experimentation->setExperimentationState(Experimentation::State::READY);
        }
        break;
    default:
        break;
    }
}

ViewerComunicationTask::~ViewerComunicationTask()
{
}
