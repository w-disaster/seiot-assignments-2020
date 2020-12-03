#ifndef __SONAR__
#define __SONAR__

#define MIN_DISTANCE 0.02
#define MAX_DISTANCE 4

class Sonar{
    public:
        virtual void init(int sonarTrigPin, int sonarEchoPin) = 0;
        virtual float getDistance() = 0;
        virtual void updateSoundSpeed() = 0;
};

#endif