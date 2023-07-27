#include "Arduino.h"
#include "MsgService.h"
#include "SerialMsgRecv.h"

String content;
MsgServiceClass MsgService;
SerialMsgRecvClass* SerialMsgRecv;

void MsgServiceClass::init(){
    Serial.begin(BAUD_RATE);
    content.reserve(256);
    content = "";
    currentMsg = "";
}

void MsgServiceClass::sendMsg(String msg){
    Serial.println(msg);
}

void serialEvent(){
    char ch;
    while (Serial.available()) {
        ch = (char) Serial.read();
        if (ch == '\n'){
            /* Create the event */
            SerialMsgRecv = new SerialMsgRecvClass(&MsgService, content);
            /* Generate it */
            MsgService.generateEvent(SerialMsgRecv);
            content = "";
        } else {
            content += ch;      
        }
    }
}


