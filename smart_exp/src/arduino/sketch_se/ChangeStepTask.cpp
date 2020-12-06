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
    step = Step::ES0;
    this->experimentationStep->setStep(ExperimentationStep::Step::READY);
    Task::init(period);
}

bool ChangeStepTask::updateTimeAndCheckEvent(int basePeriod){
    Step nextStep = step;
    bool result;
    switch (step){
    case Step::ES0:
        if (updateAndCheckTime(basePeriod)){
            nextStep = Step::ES1;
            result = true;
            break;
        }
        if (this->bStart->isPressed()){
            this->sonar->updateSoundSpeed();
            if (this->sonar->getDistance() < 0.5){
                nextStep = Step::ES3;
                init(MAX_TIME * MILLIS_TO_SEC);
            }
            else {
                nextStep = Step::ES2;
                init(ERROR_TIME * MILLIS_TO_SEC);
            }
            result = true;
            break;
        }
        result = false;
        break;

    case Step::ES1:
        /* At this point Arduino woke up from sleep mode */
        nextStep = Step::ES0;
        init(SLEEP_TIME * MILLIS_TO_SEC);
        result = true;
        break;

    case Step::ES2:
        if (updateAndCheckTime(basePeriod)){
            nextStep = Step::ES0;
            init(SLEEP_TIME * MILLIS_TO_SEC);
            result = true;
            break;
        }
        result = false;
        break;

    case Step::ES3:
        if (updateAndCheckTime(basePeriod) || this->bStop->isPressed()){
            nextStep = Step::ES4;
            result = true;
            break;
        }
        result = false;
        break;

    case Step::ES4:
        if(this->experimentationStep->getStep() == ExperimentationStep::READY){
            nextStep = Step::ES0;
            init(SLEEP_TIME * MILLIS_TO_SEC);
            result = true;
            break;
        }
        result = false;
        break;
    }
    step = nextStep;
    return result;
}

void ChangeStepTask::tick(){
    switch (step){
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
