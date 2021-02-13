#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "lib.h"

#define BUILTIN_LED 2 // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED

#define TIMER_INTERVAL_MS 2000 

/* NTP client to fetch timestamp */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);

void sendData(){
  DynamicJsonDocument data(100);
  HTTPClient http;
  http.begin(address + "/api/data");      
  http.addHeader("Content-Type", "application/json"); 

  /*float distance = measurement->getDistance();
  long timestamp = measurement->getTimestamp();
  */
  data["State"] = state;

  timeClient.update();
  timestamp = timeClient.getEpochTime();

  if(state != "NORMAL"){
    data["Distance"] = distance;
    data["Timestamp"] = timestamp;
  } else {
    data["Distance"] = "";
    data["Timestamp"] = "";
  }

  String json = "";
  serializeJson(data, json);
  Serial.println(json);
  
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

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, readDistanceAndSetState)) {
    lastMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(lastMillis);
  } else {
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
  }

  timeClient.begin();
}

void loop() { 
 if (WiFi.status()== WL_CONNECTED){   
  if(msgReady){
    sendData();
  }
 } else { 
   Serial.println("Error in WiFi connection");   
 }
 
 delay(2500);  
}
