#include "Arduino.h"
#include "StepTask.h"
#include "ButtonImpl.h"

StepTask::StepTask(Experimentation *experimentation, Sonar* sonar, int bStartPin, int bStopPin){
    this->experimentation = experimentation;
    this->sonar = sonar;
    this->bStart = new ButtonImpl(bStartPin);
    this->bStop = new ButtonImpl(bStopPin);
}

void StepTask::init(int period){
    state = State::ES0;
    this->experimentation->setExperimentationState(Experimentation::State::READY);
    Task::init(period);
}

bool StepTask::updateTimeAndCheckEvent(int basePeriod){
    State nextState = state;
    bool result;
    switch (state){
    case State::ES0:
        if (updateAndCheckTime(basePeriod)){
            nextState = State::ES1;
            result = true;
            break;
        }
        if (this->bStart->isPressed()){
            this->sonar->updateSoundSpeed();
            if (this->sonar->getDistance() < 0.5){
                nextState = State::ES3;
                init(MAX_TIME * MILLIS_TO_SEC);
            }
            else {
                nextState = State::ES2;
                init(ERROR_TIME * MILLIS_TO_SEC);
            }
            result = true;
            break;
        }
        result = false;
        break;

    case State::ES1:
        /* At this point Arduino woke up from sleep mode */
        nextState = State::ES0;
        init(SLEEP_TIME * MILLIS_TO_SEC);
        result = true;
        break;

    case State::ES2:
        if (updateAndCheckTime(basePeriod)){
            nextState = State::ES0;
            init(SLEEP_TIME * MILLIS_TO_SEC);
            result = true;
            break;
        }
        result = false;
        break;

    case State::ES3:
        if (updateAndCheckTime(basePeriod) || this->bStop->isPressed()){
            nextState = State::ES4;
            result = true;
            break;
        }
        result = false;
        break;

    case State::ES4:
        // bOK if(this->experimentation->getExperimentationState() == Experimentation::READY)
        if (/*this->bStart->isPressed() && */this->bStop->isPressed()){
            nextState = State::ES0;
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

void StepTask::tick(){
    switch (state){
        case ES0:
            this->experimentation->setExperimentationState(Experimentation::State::READY);
            break;

        case ES1:
            this->experimentation->setExperimentationState(Experimentation::State::SUSPENDED);
            break;

        case ES2:
            this->experimentation->setExperimentationState(Experimentation::State::ERROR);
            break;

        case ES3:
            this->experimentation->setExperimentationState(Experimentation::State::EXPERIMENTATION);
            break;

        case ES4:
            this->experimentation->setExperimentationState(Experimentation::State::EXPERIMENTATION_CONCLUDED);
            break;
        }
}