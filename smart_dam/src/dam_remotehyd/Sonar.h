#ifndef __SONAR__
#define __SONAR__

#define TRIG_PIN D1
#define ECHO_PIN D0

class SonarInterface {
    public:
        virtual float getDistance() = 0;
};

#endif