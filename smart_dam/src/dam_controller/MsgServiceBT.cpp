#include "MsgServiceBT.h"
#include "Arduino.h"

MsgServiceBT::MsgServiceBT(int rxPin, int txPin){
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    this->channel = new SoftwareSerial(rxPin, txPin);
}

void MsgServiceBT::init(){
    this->channel->begin(BAUD_RATE);
    this->content = "";
}

void MsgServiceBT::sendMsg(String msg){
   this->channel->println(msg);
}

void MsgServiceBT::btEvent(){
    char ch;
    while (this->channel->available()) {
        ch = (char) this->channel->read();
        if (ch == '\n'){
            /* Create the event */
            Event* ev = new BTMsgRecv(this, content);
            this->content = "";
            /* Generate it */
            this->generateEvent(ev);
        } else {
            this->content += ch;      
        }
    } 
}