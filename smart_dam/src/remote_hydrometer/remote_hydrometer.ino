#include "DataImpl.h"

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "ESP8266TimerInterrupt.h"

#define BUILTIN_LED     2       // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED

volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;

#define TIMER_INTERVAL_MS       1000

// Init ESP8266 timer 0
ESP8266Timer ITimer;

/* wifi network name */
char* ssidName = "iPhone di Luca";
/* WPA2 PSK password */
char* pwd = "lucafabri1";
/* service IP address */ 

Data* data;

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
       

void setup() { 
  Serial.begin(115200);                                
  WiFi.begin(ssidName, pwd);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.print(".");
  } 
  Serial.println("Connected: \n local IP: " + WiFi.localIP());

  data = new DataImpl("http://495ac13698c7.ngrok.io");

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, send)) {
    lastMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(lastMillis);
  } else {
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
  }
}

void loop() { 
 if (WiFi.status()== WL_CONNECTED){   

   /* read sensor */
   float value = (float) analogRead(A0) / 1023.0;
   
   /* send data */
   Serial.print("sending "+String(value)+"...");    
   //int code = sendData(address, value, "home");
   int code = 0;
   /* log result */
   if (code == 200){
     Serial.println("ok");   
   } else {
     Serial.println("error");
   }
 } else { 
   Serial.println("Error in WiFi connection");   
 }
 
 delay(5000);  
 
}
