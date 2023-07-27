#ifndef __MSG_SERVICE_BT__
#define __MSG_SERVICE_BT__
#define BT_MSG_RECV 2

#include "MsgService.h"
#include "SoftwareSerial.h"

/* Bluetooth event source */
class MsgServiceBT : public MsgEventSource {
    public: 
        MsgServiceBT(int rxPin, int txPin);
        void btEvent();
        void init();
        void sendMsg(String msg); 
    private:
        SoftwareSerial* channel;
        String content;
};

/* Bluetooth event */
class BTMsgRecv : public Event {
    public:
        BTMsgRecv(MsgServiceBT* msgServiceBT, String msg) : Event(BT_MSG_RECV, msg){
            this->source = source;  
        }

        MsgServiceBT* getSource(){
            return source;
        }

    private:
        MsgServiceBT* source;
};

#endif