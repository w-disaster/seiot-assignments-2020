#include "Arduino.h"
#include "dam_remotehyd.h"

Ticker controllerTaskTicker;
Ticker sendTaskTicker;
Ticker blinkLedTaskTicker;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

boolean msgReady;
float distance;

double d1_in_m = 1.0;
double d2_in_m = 0.4;

int sendData(){
  StaticJsonDocument<100> data;
  HTTPClient http;
  http.begin(address + "/api/data");      
  http.addHeader("Content-Type", "application/json"); 

  timeClient.update();
  long timestamp = timeClient.getEpochTime();

  /* Fetch the value of global variables to build the json */
  noInterrupts();
  data["S"] = ControllerTask->state;
  data["T"] = timestamp;
  /* At NORMAL state we mustn't send river data */
  if(ControllerTask->state != ControllerTaskClass::State::NORMAL){
    data["D"] = distance;
  }
  String json = "";
  serializeJson(data, json);
  interrupts();

  int responce = http.POST(json);    
  http.end();
  noInterrupts();
  msgReady = false;
  interrupts();
  return responce;
}