#ifndef __SONAR_IMPL__
#define __SONAR_IMPL__

#include "Sonar.h"

class SonarImpl : public Sonar{
    public:
        SonarImpl();    
        void init(int sonarTrigPin, int sonarEchoPin);
        virtual float getDistance();
};

#endif