#include "lib.h"
#include <Arduino.h>

Measurement* measurement = new MeasurementImpl();
RiverState* riverState = new RiverState();

void send(){
  HTTPClient http;
  http.begin("/api/data");      
  http.addHeader("Content-Type", "application/json"); 
  String value = "";
  String place = "";    
  String msg = String(data->getDistance()) + 
  String("{ \"value\": ") + String(value) + 
  ", \"place\": \"" + place +"\" }";
  int retCode = http.POST(msg);   
  http.end();  
    
  // String payload = http.getString();  
  // Serial.println(payload);     
}

void readDistanceAndSend(){
  
}

float getDistance(){
  return 1;
}
