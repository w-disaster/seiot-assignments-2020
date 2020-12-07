#include "Arduino.h"
#include "SonarImpl.h"
#include "ServoMotorImpl.h"
#include "KinematicsTask.h"

KinematicsTask::KinematicsTask(ExperimentationStep *experimentationStep, KinematicsData *kinematicsData, Sonar *sonar, int servoMotorPin)
{
    this->experimentationStep = experimentationStep;
    this->kinematicsData = kinematicsData;
    this->sonar = sonar;
    this->servoMotor = new ServoMotorImpl(servoMotorPin);

    this->state = K0;
    pinMode(POT_PIN, INPUT);
}

bool KinematicsTask::updateTimeAndCheckEvent(int basePeriod)
{
    State nextState = state;
    bool result;
    ExperimentationStep::Step expStep = this->experimentationStep->getStep();
    switch (state)
    {
    case K0:
        if (expStep == ExperimentationStep::Step::EXPERIMENTATION)
        {
            /* sampling rate */
            int potValue = analogRead(POT_PIN);

            /* we map the value of the potentiometer to the range 100 (ms) - 1000 (ms) and then we 
            approximate it to the nearest SCHED_PERIOD multiple */
            int period = roundToNearestMultiple(MAX_PERIOD_MS + MIN_PERIOD_MS - map(potValue, 0, 1023, MIN_PERIOD_MS, MAX_PERIOD_MS), MIN_PERIOD_MS);

            init(period);

            /* servo motor on */
            this->servoMotor->on();
            this->servoMotor->setPosition(0);

            /* start calculating speed from point x0 = 0 and acceleration from v0 = 0*/
            precDistance = 0;
            precSpeed = 0;

            /* the maximum speed is calculated every time before the experiment starts: the HC-SR04 Sonar
            can read from 2 cm distance to 400 cm and deltat is equal to the time between two samples */
            maxSpeed = (MAX_DISTANCE - MIN_DISTANCE) / (period * MS_TO_SEC);
            nextState = K1;
        }
        result = true;
        break;
    case K1:
        if (updateAndCheckTime(basePeriod))
        {
            if (expStep == ExperimentationStep::Step::EXPERIMENTATION)
            {
                result = true;
                break;
            }
            else
            {
                nextState = K0;
                this->servoMotor->setPosition(0);
                this->servoMotor->off();
                result = false;
                break;
            }
        }
        if (expStep != ExperimentationStep::Step::EXPERIMENTATION)
        {
            nextState = K0;
            this->servoMotor->setPosition(0);
            this->servoMotor->off();
        }
        result = false;
        break;
    }
    state = nextState;
    return result;
}

void KinematicsTask::tick()
{
    switch (state)
    {
    case K0:
        break;
    case K1:
        float distance = this->sonar->getDistance();
        float deltat = getPeriod() * MS_TO_SEC;
        float speed = (distance - precDistance) / deltat;
        float acceleration = (speed - precSpeed) / deltat;

        /* if it occurs, the sonar's getDistance() wasn't accurate */
        if (abs(speed) > maxSpeed)
        {
            speed = 0;
        }

        /* speed map to write to the servo motor */
        int value = mapfloat(abs(speed), 0, maxSpeed, 0, 180);

        //Serial.println(getPeriod() * 0.001 + String(":") + distance + String(":") + speed + String(":") + acceleration + String(";"));
        //Serial.println("----------------------");

        this->servoMotor->setPosition(value);

        /* write to communicator */
        this->kinematicsData->setDistance(distance);
        this->kinematicsData->setSpeed(speed);
        this->kinematicsData->setAcceleration(acceleration);
        this->kinematicsData->setDataReady(true);

        /* prec values update */
        precDistance = distance;
        precSpeed = speed;
        break;
    }
}

void KinematicsTask::init(int period)
{
    Task::init(period);
}

float KinematicsTask::mapfloat(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

int KinematicsTask::roundToNearestMultiple(int numToRound, int multiple)
{
    int remainder = numToRound % multiple;
    if (multiple == 0 || remainder == 0)
    {
        return numToRound;
    }
    if (numToRound % multiple < multiple / 2)
    {
        return numToRound - remainder;
    }
    return numToRound + multiple - remainder;
}