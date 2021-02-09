#include <ArduinoJson.h>
#include "MsgService.h"

#define LED_PIN 2

StaticJsonDocument<24> sendJson;
StaticJsonDocument<24> receivedJson;
bool connectionEstabilished;

void setup() {
  // Initialize Serial port
  connectionEstabilished = false;

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  MsgService.init();
  //while (!Serial) continue;

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document
  //
  sendJson["Mode"] = "AUTO";

  // Generate the minified JSON and send it to the Serial port.
  //
  
  // The above line prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }

  
}

void loop() {
    if (MsgService.isMsgAvailable()) {
      Msg* msg = MsgService.receiveMsg();    
      if (msg->getContent() == "ready" && !connectionEstabilished){
         delay(500);
         connectionEstabilished = true;
      } else {
        DeserializationError error = deserializeJson(receivedJson, msg->getContent());

        // Test if parsing succeeds.
        if (!error) {
          if(receivedJson["Mode"] == sendJson["Mode"]){
            digitalWrite(LED_PIN, HIGH);
            delay(1000);
            digitalWrite(LED_PIN, LOW);
          }
        }
      }
      /* NOT TO FORGET: message deallocation */
      delete msg;
    }

    if(connectionEstabilished){
        serializeJson(sendJson, Serial);
        Serial.println("");
    }
    // The above line prints:
    delay(5000);
}
