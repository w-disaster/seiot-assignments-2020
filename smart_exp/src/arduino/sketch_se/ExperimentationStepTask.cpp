#include "Arduino.h"
#include "ExperimentationStepTask.h"
#include "ButtonImpl.h"

#define CALIBRATION_TIME 10

ExperimentationStepTask::ExperimentationStepTask(Experimentation *experimentation,
                                                 int bStartPin, int bStopPin, int pir)
{
    this->experimentation = experimentation;
    this->bStart = new ButtonImpl(bStartPin);
    this->bStop = new ButtonImpl(bStopPin);
    pirPin = pir;

    pinMode(pirPin, INPUT);

    Serial.print("Calibrating PIR ");
    for (int i = 0; i < CALIBRATION_TIME; i++)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
}

void ExperimentationStepTask::init(int period)
{
    state = State::ES0;
    this->experimentation->setExperimentationState(Experimentation::State::READY);
    Task::init(period);
}

bool ExperimentationStepTask::updateTimeAndCheckEvent(int basePeriod)
{
    State nextState = state;
    bool result;
    switch (state)
    {
    case State::ES0:
        if (Task::updateAndCheckTime(basePeriod))
        {

            nextState = State::ES1;
            result = true;
            break;
        }
        if (this->bStart->isPressed())
        {
            if (digitalRead(pirPin))
            {
                nextState = State::ES3;
                init(MAX_TIME * MILLIS_TO_SEC);
            }
            else
            {
                nextState = State::ES2;
                init(ERROR_TIME * MILLIS_TO_SEC);
            }
            result = true;
            break;
        }
        result = false;
        break;

    case State::ES1:
        result = false;
        break;
    case State::ES2:
        if (Task::updateAndCheckTime(basePeriod))
        {
            nextState = State::ES0;
            init(SLEEP_TIME * MILLIS_TO_SEC);
            result = true;
            break;
        }
        result = false;
        break;
    case State::ES3:
        if (Task::updateAndCheckTime(basePeriod) || this->bStop->isPressed())
        {
            nextState = State::ES4;
            result = true;
            break;
        }
        result = false;
        break;
    case State::ES4:
        // bOK
        if (this->bStart->isPressed() && this->bStop->isPressed())
        {
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

void ExperimentationStepTask::tick()
{
    switch (state)
    {
    case ES0:
        Serial.println("andiamo in fase READY");
        this->experimentation->setExperimentationState(Experimentation::State::READY);
        break;
    case ES1:
        Serial.println("andiamo in fase SUSPENDED");

        this->experimentation->setExperimentationState(Experimentation::State::SUSPENDED);
        break;
    case ES2:
        Serial.println("andiamo in fase ERROR");

        this->experimentation->setExperimentationState(Experimentation::State::ERROR);
        break;
    case ES3:

        Serial.println("andiamo in fase EXPERIMENTATION");

        this->experimentation->setExperimentationState(Experimentation::State::EXPERIMENTATION);
        break;
    case ES4:
        Serial.println("andiamo in fase EXP CONCLUDED");

        this->experimentation->setExperimentationState(Experimentation::State::EXPERIMENTATION_CONCLUDED);
        break;
    }
}