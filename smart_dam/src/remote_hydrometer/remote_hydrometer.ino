#define ECHO_PIN D0
#define TRIG_PIN D1
#define LED_PIN D2

#include "Scheduler.h"

const float vs = 331.45 + 0.62*20;

void setup()
{
  Serial.begin(9600);

  Scheduler* scheduler = new Scheduler();
  scheduler->init(50);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT); 
  pinMode(LED_PIN, OUTPUT); 
}

float getDistance()
{
    /* invio impulso */
    digitalWrite(TRIG_PIN,LOW);
    delayMicroseconds(3);
    digitalWrite(TRIG_PIN,HIGH);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN,LOW);
    
    /* ricevi l’eco */
    float tUS = pulseIn(ECHO_PIN, HIGH);
    float t = tUS / 1000.0 / 1000.0 / 2;
    float d = t*vs;
    return d;
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
  float d = getDistance();
  Serial.println(d);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(200);

}
