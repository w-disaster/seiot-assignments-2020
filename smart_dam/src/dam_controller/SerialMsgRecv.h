#ifndef __SERIAL_MSG_RECV__
#define __SERIAL_MSG_RECV__

#define SERIAL_MSG_RECV 1

#include "async_fsm.h"
#include "MsgService.h"

/* Serial message receive event */
class SerialMsgRecvClass : public Event {
    public:
        SerialMsgRecvClass(MsgServiceClass* msgService, String msg) : Event(SERIAL_MSG_RECV, msg){
            this->source = source;  
        }

        MsgServiceClass* getSource(){
            return source;
        }

    private:
        MsgServiceClass* source;
};

extern SerialMsgRecvClass* SerialMsgRecv;

#endif