#ifndef __SONAR_IMPL__
#define __SONAR_IMPL__

#include "Sonar.h"

/* Sonar class */
class SonarClass : public SonarInterface {
    int trigPin;
    int echoPin;
    float vs;
    
    public:
        SonarClass(int sonarTrigPin, int sonarEchoPin);    
        float getDistance();
};

extern SonarClass* Sonar;

#endif