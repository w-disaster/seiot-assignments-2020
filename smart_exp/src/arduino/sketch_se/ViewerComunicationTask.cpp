#include "ViewerComunicationTask.h"
#include "Arduino.h"

ViewerComunicationTask::ViewerComunicationTask(ExperimentationStep *experimentationStep, KinematicsData *kinematicsData)
{
    Serial.begin(115200);
    this->experimentationStep = experimentationStep;
    this->kinematicsData = kinematicsData;
    this->stepMsgAlreadySent = false;
    this->currentExpStep = this->experimentationStep->getStep();
}

void ViewerComunicationTask::sendData(String msg, bool isStep)
{
    isStep ? Serial.print(msg + STEP_END + MSG_END) : Serial.print(msg + MSG_END);
}

void ViewerComunicationTask::sendStepMsgOnce(String stepKey)
{
    if (!stepMsgAlreadySent)
    {
        /* sends the Step to serial */
        sendData(stepKey, true);

        if (step == VC1)
        {
            /* start the relative clock if we are experimenting */
            expRelativeTime = micros();
        }
        stepMsgAlreadySent = true;
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
    this->step = VC0;
    tick();
    Task::init(period);
}

bool ViewerComunicationTask::updateTimeAndCheckEvent(int basePeriod)
{
    bool result = false;
    Step nextStep = step;
    ExperimentationStep::Step expStep = this->experimentationStep->getStep();

    switch (step)
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
                    nextStep = VC1;
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
                nextStep = VC2;
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
                nextStep = VC0;
                this->stepMsgAlreadySent = false;
                /* update expStep */
                this->currentExpStep = expStep;
            }
            /* keep ticking */
            result = true;
        }
        break;
    }
    step = nextStep;
    return result;
}

void ViewerComunicationTask::tick()
{
    switch (step)
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


