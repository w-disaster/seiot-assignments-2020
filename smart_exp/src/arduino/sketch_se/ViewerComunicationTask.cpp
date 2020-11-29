#include "ViewerComunicationTask.h"

ViewerComunicationTask::ViewerComunicationTask(Experimentation *experimentation)
{
    this->experimentation = experimentation;
    this->stateMsgAlreadySent = false;
    this->state = VC0;
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
}

String ViewerComunicationTask::format(long t, float p, float v, float a)
{
    String formattedString = String(t) + DATA_SEPARATOR + String(p) + DATA_SEPARATOR + String(v) + DATA_SEPARATOR + String(a);
    return formattedString;
}

void ViewerComunicationTask::init(int period)
{
    Task::init(period);
}

bool ViewerComunicationTask::updateTimeAndCheckEvent(int basePeriod)
{
    bool result = false;
    switch (state)
    {
    case VC0:
        /* ready */
        if (this->experimentation->getExperimentationState() == Experimentation::State::SUSPENDED)
        {
            /* goes to suspended */
            this->state = VC1;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        else if (this->experimentation->getExperimentationState() == Experimentation::State::ERROR)
        {
            /* goes to error */
            this->state = VC2;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        else if (this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION)
        {
            /* goes to exp */
            this->state = VC3;
            this->stateMsgAlreadySent = false;
            result = true;
        }

        break;
    case VC1:
        /* suspended */
        if (this->experimentation->getExperimentationState() == Experimentation::State::READY)
        {
            /* goes to ready */
            this->state = VC0;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;
    case VC2:
        /* error */
        if (this->experimentation->getExperimentationState() == Experimentation::State::READY)
        {
            /* goes to ready */
            this->state = VC0;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;
    case VC3:
        /* exp */
        if (this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION_CONCLUDED)
        {
            /* goes to exp over */
            this->state = VC4;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;
    case VC4:
        /* exp over */
        if (this->experimentation->getExperimentationState() == Experimentation::State::READY)
        {
            /* goes to ready */
            this->state = VC0;
            this->stateMsgAlreadySent = false;
            result = true;
        }
        break;
    default:
        break;
    }

    return result;
}

void ViewerComunicationTask::tick()
{
    switch (state)
    {
    case VC0:
        /* ready */
        sendStateMsgOnce("ready");
        break;
    case VC1:
        /* suspended */
        sendStateMsgOnce("suspended");
        break;
    case VC2:
        /* error */
        sendStateMsgOnce("error");
        break;
    case VC3:
        /* exp */
        sendStateMsgOnce("exp");
        sendExperimentData(format(micros() - this->expRelativeTime,
                                  this->experimentation->getDistance(),
                                  this->experimentation->getSpeed(),
                                  this->experimentation->getAcceleration()));
        break;
    case VC4:
        /* exp over */
        sendStateMsgOnce("over");
        break;
    default:
        break;
    }
}

ViewerComunicationTask::~ViewerComunicationTask()
{
}
