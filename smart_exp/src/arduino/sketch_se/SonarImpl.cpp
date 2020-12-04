#include "SonarImpl.h"
#include "Arduino.h"

SonarImpl::SonarImpl(int sonarTrigPin, int sonarEchoPin){
    trigPin = sonarTrigPin;
    echoPin = sonarEchoPin;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT); 
}

void SonarImpl::init(){
    dht.begin();
}

float SonarImpl::getDistance(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(3);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);

    float tUS = pulseIn(echoPin, HIGH);
    float t = tUS / 1000.0 / 1000.0 / 2;
    float d = t * vs;
    return d;
}

void SonarImpl::updateSoundSpeed(){
    float temperature = dht.readTemperature();
    Serial.println(temperature);
    vs = 331.45 + 0.62 * temperature;
}