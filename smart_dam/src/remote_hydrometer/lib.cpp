#include "lib.h"
#include <Arduino.h>

const double vs = 331.45 + 0.62*20;

volatile boolean statusLed = false;
volatile uint32_t lastMillis = 0;

String state;
volatile float distance;
unsigned long timestamp;
volatile boolean msgReady = false;

char* ssidName = "Vodafonefabri";
char* pwd = "3Com2007";
String address = "http://192.168.1.5:8080";

ESP8266Timer ITimer;

/*
void sendData(){
  DynamicJsonDocument data(100);
  HTTPClient http;
  http.begin(address + "/api/data");      
  http.addHeader("Content-Type", "plain/text"); 

  RiverState::State state = riverState->getState();
  float distance = measurement->getDistance();
  long timestamp = measurement->getTimestamp();
  
  data["State"] = state;

  if(state == RiverState::State::NORMAL){
    data["Distance"] = distance;
    data["Timestamp"] = timestamp;
  } else {
    data["Distance"] = "";
    data["Timestamp"] = "";
  }

  String json = "";
  serializeJson(data, json);
  
  int retCode = http.POST(json);   
  http.end();
  msgReady = false;
}*/


void ICACHE_RAM_ATTR readDistanceAndSetState(){
  /* Distance read from HC-SR04 and measurement fields fill*/
  distance = getDistance();

  /* We determine the State by the distance sampled */
  if(distance > D1_IN_M){
    state = "NORMAL";
  } else if(distance >= D1_IN_M && distance < D2_IN_M){
    state = "PRE_ALARM";
  } else {
    state = "ALARM";
  }
  
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

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.println("Delta ms = " + String(millis() - lastMillis));
  lastMillis = millis();
#endif
}
