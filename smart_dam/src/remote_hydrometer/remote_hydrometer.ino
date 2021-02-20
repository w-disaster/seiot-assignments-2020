#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include "lib.h"

Ticker blinker;
Ticker dataReader;
Ticker senderController;

boolean mustDetach;

void setup() { 
  Serial.begin(115200);    

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT); 
  pinMode(LED_PIN, OUTPUT); 

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

  mustDetachLedISR = false;

  /* Client start to fetch current timestamp */
  timeClient.begin();
}

void setLedBehaviour(State state){
  noInterrupts();
  switch(state){
    case State::NORMAL:
        /* BlinkLed interrupt detach if senderController tells us */
        if(mustDetachLedISR){
          blinker.detach();
        }
        /* At NORMAL state the Led is off */ 
        digitalWrite(LED_PIN, LOW);
        break;
      case State::PREALARM:
        /* If the new state is PRE_ALARM then we attach the interrupt
         *  that every 200ms switchs on or off the led
         */
        blinker.attach_ms(200, blinkLed);
        break;
      case State::ALARM:
        if(mustDetachLedISR){
          blinker.detach();
        }
        /* At ALARM state the Led is steady on */
        digitalWrite(LED_PIN, HIGH);
        break;
  }
  interrupts();
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

          /* If state is changed, we must edit the Led behaviour */
          setLedBehaviour(state);
          sendData();
        }
        break;
      case State::PREALARM:
        /* If the state is different from the previous sampled by dataReader ISR,
         *  we attach the one to senderController, that sets the msg ready to be sent
         *  every T seconds, depending on the state
         */
        if(isStateChanged){
          senderController.attach(10, setMsgReady);
          mustDetach = true;
          /* If state is changed, we must edit the Led behaviour */
          setLedBehaviour(state);
        }
        /* We send data at state change and when senderController tells us */
        if(msgReady || isStateChanged){
          Serial.println(String("Sending ") + (char)state);
          sendData();
        }
        break;
      case State::ALARM:
        /* If the state is different from the previous sampled by dataReader ISR,
         *  we attach the one to senderController, that sets the msg ready to be sent
         *  every T seconds, depending on the state
         */
        if(isStateChanged){
          senderController.attach(5, setMsgReady);
          mustDetach = true;
          /* If state is changed, we must edit the Led behaviour */
          setLedBehaviour(state);
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
