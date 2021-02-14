#ifndef __MSGSERVICEBT__
#define __MSGSERVICEBT__

#include "SoftwareSerial.h"
#include "Msg.h"

class MsgServiceBT
{

public:
  MsgServiceBT(int rxPin, int txPin);
  void init();
  bool isMsgAvailable();
  Msg *receiveMsg();
  bool sendMsg(Msg msg);

private:
  String content;
  Msg *availableMsg;
  SoftwareSerial *channel;
};

#endif
