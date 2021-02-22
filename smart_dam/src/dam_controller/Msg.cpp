#include "Arduino.h"
#include "Msg.h"

Msg::Msg(String content){
    this->content = content;
    setMsgReady(true);
}
  
String Msg::getContent(){
    return this->content;
}

void Msg::setContent(String content){
    this->content = content;
}


void Msg::setMsgReady(bool flag){
    this->isReady = flag;
}
    
bool Msg::isMsgReady(){
    return this->isReady;
}
