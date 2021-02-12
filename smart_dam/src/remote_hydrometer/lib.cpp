#include "lib.h"
#include <Arduino.h>

Measurement* measurement = new MeasurementImpl();
RiverState* riverState = new RiverState();
const double vs = 331.45 + 0.62*20;
StaticJsonDocument<100> data;

void sendData(){
  HTTPClient http;
  http.begin("/api/data");      
  http.addHeader("Content-Type", "application/json"); 
  
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
  
  String json;
  serializeJson(data, json);

  int retCode = http.POST(json);   
  http.end();  
    
  // String payload = http.getString();  
  // Serial.println(payload);     
}

void readDistanceAndSend(){
  /* Distance read from HC-SR04 and measurement fields fill*/
  float distance = getDistance();
  measurement->setDistance(distance);
  measurement->setTimestamp(millis());

  /* We determine the State by the distance sampled */
  if(distance > D1_IN_M){
    riverState->setState(RiverState::State::NORMAL);
  } else if(distance >= D1_IN_M && distance < D2_IN_M){
    riverState->setState(RiverState::State::PRE_ALARM);
  } else {
    riverState->setState(RiverState::State::ALARM);
  }

  /* The data is ready to be sent */
  sendData();
}

float getDistance(){
  /* invio impulso */
    digitalWrite(TRIG_PIN,LOW);
    delayMicroseconds(3);
    digitalWrite(TRIG_PIN,HIGH);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN,LOW);
    
    /* ricevi l’eco */
    long tUS = pulseInLong(ECHO_PIN, HIGH);
    Serial.println(tUS);

    double t = tUS / 1000.0 / 1000.0 / 2;
    double d = t*vs;
    return d;
}
