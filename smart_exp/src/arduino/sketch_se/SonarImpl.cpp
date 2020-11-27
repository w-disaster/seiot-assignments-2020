#include "SonarImpl.h"
#include "Arduino.h"

SonarImpl::SonarImpl(){}

void SonarImpl::init(int sonarTrigPin, int sonarEchoPin){
    Sonar::init(sonarTrigPin, sonarEchoPin);
}

float SonarImpl::getDistance(){
    Sonar::getDistance();
}