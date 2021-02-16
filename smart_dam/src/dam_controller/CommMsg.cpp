#include "Arduino.h"
#include "CommMsg.h"

void CommMsg::setMsgReady(bool flag){
  this->msgReady = flag;
}

bool CommMsg::isMsgReady(){
  return this->msgReady;
}
