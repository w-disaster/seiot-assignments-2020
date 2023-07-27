#include "Arduino.h"
#include "dam_remotehyd.h"
#include "config.h"

/* 
 * @author: Luca Fabri
*/

void setup(){
    Serial.begin(BAUD_RATE);    
    
    /* Connect via Wi-Fi */
    WiFi.begin(ssidName, pwd);
    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) {  
        delay(500);
        Serial.print(".");
    } 
    Serial.println("Connected: \n local IP: " + WiFi.localIP());

    /* Client start to fetch current timestamp */
    timeClient.setTimeOffset(3600 * 2);
    timeClient.begin();

    /* Initial state of the Led:OFF */
    Led->switchOff();
    /* Set the period of the blink led task */
    BlinkLedTask->init(BL_TASK_T);
    /* Set the period of the controller task and attach the interrupt */
    ControllerTask->init(CTR_TASK_T);
    ControllerTask->attachInt();
}

void loop(){
    if (WiFi.status()== WL_CONNECTED){  
        /* Send the data when a task set that a message is ready to be sent */
        if(msgReady){
            int code = sendData();
            Serial.println("Return code from DS: " + String(code));
        }    
    }
}