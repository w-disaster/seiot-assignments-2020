#ifndef __SONAR_IMPL__
#define __SONAR_IMPL__

#include "Sonar.h"
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11

class SonarImpl : public Sonar{
    DHT dht = DHT(DHTPIN, DHTTYPE);
    int trigPin;
    int echoPin;
    float vs;
    
    public:
        SonarImpl();    
        void init(int sonarTrigPin, int sonarEchoPin);
        float getDistance();
        void updateSoundSpeed();
};

#endif