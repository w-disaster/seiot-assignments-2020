#include "ViewerComunicationTask.h"
#include "Arduino.h"

ViewerComunicationTask::ViewerComunicationTask(Experimentation *experimentation, KinematicsData *kinematicsData)
{
    Serial.begin(115200);
    this->experimentation = experimentation;
    this->kinematicsData = kinematicsData;
    this->stateMsgAlreadySent = false;
    this->currentExpState = this->experimentation->getExperimentationState();
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

        if (state == VC1)
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
    tick();
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
        if (updateAndCheckTime(basePeriod))
        {
            /* if current is not the same we go in the next accordingly */
            if (expState != currentExpState)
            {
                /* send new state */
                this->stateMsgAlreadySent = false;
                /* update expState */
                this->currentExpState = expState;
                /* return true */
                result = true;

                if(this->currentExpState == Experimentation::State::EXPERIMENTATION){
                    nextState = VC1;
                    break;
                }
            }
        }
        break;
    case VC1:
        /* exp */
        if (updateAndCheckTime(basePeriod))
        {
            if (expState == Experimentation::State::EXPERIMENTATION_CONCLUDED)
            {
                /* goes to exp over */
                nextState = VC2;
                this->stateMsgAlreadySent = false;
                /* update expState */
                this->currentExpState = expState;
            }
            /* keep ticking */
            result = true;
        }
        break;

    case VC2:
        /* exp over */
        if (updateAndCheckTime(basePeriod))
        {
            if (expState == Experimentation::State::READY)
            {
                /* goes to ready */
                nextState = VC0;
                this->stateMsgAlreadySent = false;
                /* update expState */
                this->currentExpState = expState;
            }
            /* keep ticking */
            result = true;
        }
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
        switch (this->currentExpState){
            case Experimentation::State::READY:
                sendStateMsgOnce("ready");
                break;

            case Experimentation::State::SUSPENDED:
                sendStateMsgOnce("suspended");
                delay(5);
                break;

            case Experimentation::State::ERROR:
                sendStateMsgOnce("error");
                break;
        }
        break;
    case VC1:
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
    case VC2:
        /* exp over */
        sendStateMsgOnce("over");        
        if (Serial.available() > 0)
        {
            if ((char)Serial.read() == EXP_OVER)
            {
                /* tells experimentation to change State */
                this->experimentation->setExperimentationState(Experimentation::State::READY);
            }
        }
        break;
    }
}


