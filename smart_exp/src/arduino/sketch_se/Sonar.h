#ifndef __SONAR__
#define __SONAR__

class Sonar{
    public:
        virtual void init(int sonarTrigPin, int sonarEchoPin) = 0;
        virtual float getDistance() = 0;
        virtual void updateSoundSpeed() = 0;
};

#endif