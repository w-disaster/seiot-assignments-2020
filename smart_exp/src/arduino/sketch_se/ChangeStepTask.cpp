#include "Arduino.h"
#include "ChangeStepTask.h"
#include "ButtonImpl.h"

ChangeStepTask::ChangeStepTask(ExperimentationStep *experimentationStep, Sonar* sonar, int bStartPin, int bStopPin){
    this->experimentationStep = experimentationStep;
    this->sonar = sonar;
    this->bStart = new ButtonImpl(bStartPin);
    this->bStop = new ButtonImpl(bStopPin);
}

void ChangeStepTask::init(int period){
    state = State::ES0;
    this->experimentationStep->setStep(ExperimentationStep::Step::READY);
    Task::init(period);
}

bool ChangeStepTask::updateTimeAndCheckEvent(int basePeriod){
    State nextState = state;
    bool result;
    switch (state){
    case ES0:
        if (updateAndCheckTime(basePeriod)){
            nextState = ES1;
            result = true;
            break;
        }
        if (this->bStart->isPressed()){
            this->sonar->updateSoundSpeed();
            if (this->sonar->getDistance() < 0.5){
                nextState = ES3;
                init(MAX_TIME * MILLIS_TO_SEC);
            }
            else {
                nextState = ES2;
                init(ERROR_TIME * MILLIS_TO_SEC);
            }
            result = true;
            break;
        }
        result = false;
        break;

    case ES1:
        /* At this point Arduino woke up from sleep mode */
        nextState = ES0;
        init(SLEEP_TIME * MILLIS_TO_SEC);
        result = true;
        break;

    case ES2:
        if (updateAndCheckTime(basePeriod)){
            nextState = ES0;
            init(SLEEP_TIME * MILLIS_TO_SEC);
            result = true;
            break;
        }
        result = false;
        break;

    case ES3:
        if (updateAndCheckTime(basePeriod) || this->bStop->isPressed()){
            nextState = ES4;
            result = true;
            break;
        }
        result = false;
        break;

    case ES4:
        if(this->experimentationStep->getStep() == ExperimentationStep::READY){
            nextState = ES0;
            init(SLEEP_TIME * MILLIS_TO_SEC);
            result = true;
            break;
        }
        result = false;
        break;
    }
    state = nextState;
    return result;
}

void ChangeStepTask::tick(){
    switch (state){
        case ES0:
            this->experimentationStep->setStep(ExperimentationStep::Step::READY);
            break;

        case ES1:
            this->experimentationStep->setStep(ExperimentationStep::Step::SUSPENDED);
            break;

        case ES2:
            this->experimentationStep->setStep(ExperimentationStep::Step::ERROR);
            break;

        case ES3:
            this->experimentationStep->setStep(ExperimentationStep::Step::EXPERIMENTATION);
            break;

        case ES4:
            this->experimentationStep->setStep(ExperimentationStep::Step::EXPERIMENTATION_CONCLUDED);
            break;
        }
}
