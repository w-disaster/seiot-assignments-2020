#ifndef __BT_COMM_TASK__
#define __BT_COMM_TASK__

#include "Task.h"
#include "Msg.h"
#include "MsgServiceBT.h"

#define RX 2
#define TX 3

class BTCommTask : public Task{

    MsgServiceBT* btService;
    Msg* serialMsg;
    Msg* btMsg;

    enum State{
        BT0, // no activity: waiting messages
        BT1, // msg from Serial task
        BT2, // msg from bluetooth
        BT3  // msg from Serial and bluetooth
    };
    State state;
    
    public:
        BTCommTask(Msg* serialMsg, Msg* btMsg);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};


#endif
