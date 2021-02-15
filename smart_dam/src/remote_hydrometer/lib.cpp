#include "lib.h"
#include <Arduino.h>

const double vs = 331.45 + 0.62*20;

volatile boolean statusLed = false;
volatile uint32_t lastMillis = 0;

float distance;
unsigned long timestamp;
boolean msgReady = false;
boolean isStateChanged; 
boolean mustDetachLedISR;

State state;
State precState;

/* NTP client to fetch timestamp */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);

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
    /* If the state is changed and the previous was PRE_ALARM then
     *  we must detach the interrupt
     */
    if(precState == State::PRE_ALARM){
      mustDetachLedISR = true;
    } else {
      mustDetachLedISR = false;
    }
    isStateChanged = true;
  } else {
    mustDetachLedISR = false;
    isStateChanged = false;
  }

  precState = state;
}

void sendData(){
  DynamicJsonDocument data(100);
  HTTPClient http;
  http.begin(address + "/api/data");      
  http.addHeader("Content-Type", "application/json"); 

  timeClient.update();
  timestamp = timeClient.getEpochTime();

  noInterrupts();
  /* We determine the strint to send */
  switch(state){
    case State::NORMAL:
        data["State"] = "NORMAL";
        break;
      case State::PRE_ALARM:
        data["State"] = "PRE_ALARM";
        break;
      case State::ALARM:
        data["State"] = "ALARM";
        break;
  }

  /* At NORMAL state we mustn't send river data */
  if(state != State::NORMAL){
    data["Distance"] = distance;
    data["Timestamp"] = timestamp;
  }
  interrupts();
  
  String json = "";
  serializeJson(data, json);
  //Serial.println(json);
  
  http.POST(json);   
  http.end();
  
  msgReady = false;
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

void blinkLed() {
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(LED_PIN, OUTPUT);
  }

  digitalWrite(LED_PIN, statusLed);  //Toggle LED Pin
  statusLed = !statusLed;
}
