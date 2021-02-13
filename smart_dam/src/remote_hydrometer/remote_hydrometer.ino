#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include "lib.h"

/* NTP client to fetch timestamp */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);
Ticker blinker;
Ticker dataReader;
Ticker senderController;

boolean mustDetach;

void sendData(){
  DynamicJsonDocument data(100);
  HTTPClient http;
  http.begin(address + "/api/data");      
  http.addHeader("Content-Type", "application/json"); 

  timeClient.update();
  timestamp = timeClient.getEpochTime();

  noInterrupts();
  data["State"] = state;

  if(state != State::NORMAL){
    data["Distance"] = distance;
    data["Timestamp"] = timestamp;
  } else {
    data["Distance"] = "";
    data["Timestamp"] = "";
  }
  interrupts();
  
  String json = "";
  serializeJson(data, json);
  //Serial.println(json);
  
  http.POST(json);   
  http.end();
  
  msgReady = false;
}

void setup() { 
  Serial.begin(115200);    

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);  

  WiFi.begin(ssidName, pwd);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.print(".");
  } 
  Serial.println("Connected: \n local IP: " + WiFi.localIP());

  /* We determine the state of the river */
  precState = State::IDLE;
  readDistanceAndSetState();
  /* We attach an interrupt that reads distance and sets state every second */
  dataReader.attach(1, readDistanceAndSetState);

  /* If state sampled is NORMAL then we cannot detach the Timer that 
   *  sends data to the server
   */
  mustDetach = false;
  if(state != State::NORMAL){
    mustDetach = true;
  }

  /* Client start to fetch current timestamp */
  timeClient.begin();
}

void loop() { 
  if (WiFi.status() == WL_CONNECTED){ 
    noInterrupts();
    switch(state){
      case State::NORMAL:
        /* At state change we always send the data, even in the other cases */
        if(isStateChanged){
          /* As said before, we detach the interrupt if we're coming from
           *  PRE_ALARM or ALARM state
           */
          if(mustDetach){
            senderController.detach();
          }
          Serial.println(String("Sending ") + (char)state);
          sendData();
        }
        break;
      case State::PRE_ALARM:
      case State::ALARM:
        /* If the state is different from the previous sampled by dataReader ISR,
         *  we attach the one to senderController, that sets the msg ready to be sent
         *  every T seconds, depending on the state
         */
        if(isStateChanged){
          senderController.attach(state == State::ALARM ? 5 : 10, setMsgReady);
          mustDetach = true;
        }
        /* We send data at state change and when senderController tells us */
        if(msgReady || isStateChanged){
          Serial.println(String("Sending ") + (char)state);
          sendData();
        }
        break;
    }
    interrupts();
    
 } else { 
   Serial.println("Error in WiFi connection");   
 }
 
 delay(1000);  
}
