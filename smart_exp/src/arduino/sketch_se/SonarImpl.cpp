#include "SonarImpl.h"
#include "Arduino.h"

SonarImpl::SonarImpl(){}

void SonarImpl::init(int sonarTrigPin, int sonarEchoPin){
    trigPin = sonarTrigPin;
    echoPin = sonarEchoPin;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT); 
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
    float temperature =  dht.readTemperature();
    Serial.print("temperature: ");
    Serial.println(temperature);
    vs = 331.45 + 0.62 * temperature;
}