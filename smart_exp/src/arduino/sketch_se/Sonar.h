#ifndef __SONAR__
#define __SONAR__

#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11

class Sonar{
    DHT dht = DHT(DHTPIN, DHTTYPE);
    int trigPin;
    int echoPin;
    
    public:
        virtual void init(int sonarTrigPin, int sonarEchoPin){
            trigPin = sonarTrigPin;
            echoPin = sonarEchoPin;
            pinMode(trigPin, OUTPUT);
            pinMode(echoPin, INPUT); 
            dht.begin(); 
        }

        virtual float getDistance(){
            digitalWrite(trigPin,LOW);
            delayMicroseconds(3);
            digitalWrite(trigPin,HIGH);
            delayMicroseconds(5);
            digitalWrite(trigPin,LOW);
            
            /* sound speed */
            float vs = 331.45 + 0.62 * dht.readTemperature();

            float tUS = pulseIn(echoPin, HIGH);
            float t = tUS / 1000.0 / 1000.0 / 2;
            float d = t*vs;
            return d;
        }
};

#endif