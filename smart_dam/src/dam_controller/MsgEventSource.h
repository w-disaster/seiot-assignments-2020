#ifndef __MSG_EVENT_SOURCE__
#define __MSG_EVENT_SOURCE__

#include "async_fsm.h"

/* Event source class */
class MsgEventSource : public EventSource {
    public: 
        String currentMsg;

        virtual void init() = 0;
        virtual void sendMsg(String msg) = 0;
}; 

#endif