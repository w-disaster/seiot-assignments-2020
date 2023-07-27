#include "SonarClass.h"
#include "Arduino.h"

SonarClass* Sonar = new SonarClass(TRIG_PIN, ECHO_PIN);

SonarClass::SonarClass(int sonarTrigPin, int sonarEchoPin)
{
    vs = 331.45 + 0.62*20;
    trigPin = sonarTrigPin;
    echoPin = sonarEchoPin;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float SonarClass::getDistance()
{
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