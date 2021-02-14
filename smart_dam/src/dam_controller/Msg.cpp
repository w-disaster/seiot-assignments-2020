#include "Arduino.h"
#include "Msg.h"

Msg::Msg(String content){
  this->content = content;
}
  
String Msg::getContent(){
  return this->content;
}
