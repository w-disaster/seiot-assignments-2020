#ifndef __COMM_MSG__
#define __COMM_MSG__

#include "Msg.h"

class CommMsg : public Msg {
  bool msgReady;

  public:
    CommMsg(String content) : Msg(content){};
    void setMsgReady(bool flag);
    bool isMsgReady();
  
};


#endif
