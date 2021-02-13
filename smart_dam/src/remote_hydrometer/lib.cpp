#include "lib.h"
#include <Arduino.h>

const double vs = 331.45 + 0.62*20;

volatile boolean statusLed = false;
volatile uint32_t lastMillis = 0;

float distance;
unsigned long timestamp;
boolean msgReady = false;
boolean isStateChanged; 

State state;
State precState;

char* ssidName = "Vodafonefabri";
char* pwd = "3Com2007";
String address = "http://192.168.1.5:8080";

void readDistanceAndSetState(){
  /* Distance read from HC-SR04 and measurement fields fill*/
  distance = getDistance();
  Serial.println(String("distance: ") + distance);
  /* We determine the State by the distance sampled */
  if(distance > D1_IN_M){
    state = State::NORMAL;
  } else if(distance >= D2_IN_M && distance <= D1_IN_M){
    state = State::PRE_ALARM;
  } else {
    state = State::ALARM;
  }
  if(precState != state){
    isStateChanged = true;
  } else {
    isStateChanged = false;
  }
  precState = state;
}

void setMsgReady(){
  Serial.println("setting msg ready");
  /* The data is ready to be sent */
  msgReady = true;
}

float getDistance(){
  /* invio impulso */
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(3);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, LOW);
    
    /* ricevi l’eco */
    long tUS = pulseInLong(ECHO_PIN, HIGH);

    double t = tUS / 1000.0 / 1000.0 / 2;
    double d = t*vs;
    return d;
}

void ICACHE_RAM_ATTR blinkLed() {
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(BUILTIN_LED, OUTPUT);
  }

  digitalWrite(BUILTIN_LED, statusLed);  //Toggle LED Pin
  statusLed = !statusLed;
}
