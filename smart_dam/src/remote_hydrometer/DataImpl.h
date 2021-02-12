#ifndef __DATA_IMPL__
#define __DATA_IMPL__

#include "Data.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "lib.h"

class DataImpl : public Data {
  public:
    static String address;
  
    DataImpl(String address);
    void setDistance(float d);
    float getDistance();
  
    static void ICACHE_RAM_ATTR send(){
       HTTPClient http;    
       http.begin("/api/data");      
       http.addHeader("Content-Type", "application/json"); 
       String value = "";
       String place = "";    
       String msg = String(measurement->getTimestamp()) + 
        String("{ \"value\": ") + String(value) + 
        ", \"place\": \"" + place +"\" }";
       int retCode = http.POST(msg);   
       http.end();  
          
       // String payload = http.getString();  
       // Serial.println(payload);      
    }

    
};

#endif
