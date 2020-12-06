#include "ViewerCommunicatorTask.h"
#include "Arduino.h"

ViewerCommunicatorTask::ViewerCommunicatorTask(ExperimentationStep *experimentationStep, KinematicsData *kinematicsData)
{
    Serial.begin(115200);
    this->experimentationStep = experimentationStep;
    this->kinematicsData = kinematicsData;
    this->stepMsgAlreadySent = false;
    this->currentExpStep = this->experimentationStep->getStep();
}

void ViewerCommunicatorTask::sendData(String msg, bool isStep)
{
    isStep ? Serial.print(msg + STEP_END + MSG_END) : Serial.print(msg + MSG_END);
}

void ViewerCommunicatorTask::sendStepMsgOnce(String stepKey)
{
    if (!stepMsgAlreadySent)
    {
        /* sends the Step to serial */
        sendData(stepKey, true);

        if (state == VC1)
        {
            /* start the relative clock if we are experimenting */
            expRelativeTime = micros();
        }
        stepMsgAlreadySent = true;
    }
}

void ViewerCommunicatorTask::sendExperimentData(String data)
{
    /* sends msg ad experiment data */
    sendData(data, false);
    this->kinematicsData->setDataReady(false);
}

String ViewerCommunicatorTask::format(float t, float p, float v, float a)
{
    String formattedString = String(t) + DATA_SEPARATOR + String(p) + DATA_SEPARATOR + String(v) + DATA_SEPARATOR + String(a);
    return formattedString;
}

void ViewerCommunicatorTask::init(int period)
{
    this->state = VC0;
    tick();
    Task::init(period);
}

bool ViewerCommunicatorTask::updateTimeAndCheckEvent(int basePeriod)
{
    bool result = false;
    State nextState = state;
    ExperimentationStep::Step expStep = this->experimentationStep->getStep();

    switch (state)
    {
    case VC0:
        if (updateAndCheckTime(basePeriod))
        {
            /* if current is not the same we go in the next accordingly */
            if (expStep != currentExpStep)
            {
                /* send new step */
                this->stepMsgAlreadySent = false;
                /* update expStep */
                this->currentExpStep = expStep;
                /* return true */
                result = true;

                if(this->currentExpStep == ExperimentationStep::Step::EXPERIMENTATION){
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
            if (expStep == ExperimentationStep::Step::EXPERIMENTATION_CONCLUDED)
            {
                /* goes to exp over */
                nextState = VC2;
                this->stepMsgAlreadySent = false;
                /* update expStep */
                this->currentExpStep = expStep;
            }
            /* keep ticking */
            result = true;
        }
        break;

    case VC2:
        /* exp over */
        if (updateAndCheckTime(basePeriod))
        {
            if (expStep == ExperimentationStep::Step::READY)
            {
                /* goes to ready */
                nextState = VC0;
                this->stepMsgAlreadySent = false;
                /* update expStep */
                this->currentExpStep = expStep;
            }
            /* keep ticking */
            result = true;
        }
        break;
    }
    state = nextState;
    return result;
}

void ViewerCommunicatorTask::tick()
{
    switch (state)
    {
    case VC0:
        switch (this->currentExpStep){
            case ExperimentationStep::Step::READY:
                sendStepMsgOnce("ready");
                break;

            case ExperimentationStep::Step::SUSPENDED:
                sendStepMsgOnce("suspended");
                delay(5);
                break;

            case ExperimentationStep::Step::ERROR:
                sendStepMsgOnce("error");
                break;
        }
        break;
    case VC1:
        /* exp */
        sendStepMsgOnce("exp");
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
        sendStepMsgOnce("over");        
        if (Serial.available() > 0)
        {
            if ((char)Serial.read() == EXP_OVER)
            {
                /* tells experimentationStep to change Step */
                this->experimentationStep->setStep(ExperimentationStep::Step::READY);
            }
        }
        break;
    }
}


