#include "Arduino.h"
#include "KinematicsTask.h"

KinematicsTask::KinematicsTask(Experimentation* experimentation, int trigPin, int echoPin){
    this->experimentation = experimentation;
    this->sonar = new Sonar();
    this->sonar->init(trigPin, echoPin);
    this->state = K0;

    pinMode(POT_PIN, INPUT);
}

bool KinematicsTask::updateTimeAndCheckEvent(int basePeriod){
    switch(state){
        case K0:
            if(this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION){
                /* sampling rate */
                int potValue = digitalRead(POT_PIN);

                int period = roundToNearestMultiple(1050 - map(potValue, 0, 1023, 50, 1000), 50);
                init(period);
                Serial.println(String("periodo task cinematica: ") + period + "pot value: " + potValue);

                state = K1;
            }
            return true;
        case K1:
            if(Task::updateAndCheckTime(basePeriod)){
                if(this->experimentation->getExperimentationState() == Experimentation::State::EXPERIMENTATION){
                    return true;
                } else {
                    state = K0;
                    return false;
                }
            }
            if(this->experimentation->getExperimentationState() != Experimentation::State::EXPERIMENTATION){
                state = K0;
            }
            return false;
    }
}

void KinematicsTask::tick(){
    switch(state){
        case K0:
            break;
        case K1:
            Serial.println(String("distanza: ") + this->sonar->getDistance());
            break;
    }
}

void KinematicsTask::init(int period){
    Task::init(period);
}

int KinematicsTask::roundToNearestMultiple(int numToRound, int multiple){
    int remainder = numToRound % multiple;
    if(multiple == 0 || remainder == 0){
        return numToRound;
    }
    if(numToRound % multiple < multiple / 2){
        return numToRound - remainder;
    }
    return numToRound + multiple - remainder;
}