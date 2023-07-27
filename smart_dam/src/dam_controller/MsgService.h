#ifndef __MSG_SERVICE__
#define __MSG_SERVICE__

#include "MsgEventSource.h"
#define BAUD_RATE 9600

/* Serial message service */
class MsgServiceClass : public MsgEventSource {
    public: 
        void init();
        void sendMsg(String msg); 
}; 

extern MsgServiceClass MsgService;

#endif