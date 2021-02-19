#ifndef __DAM_SERVICE_COMM_TASK__
#define __DAM_SERVICE_COMM_TASK__

#include "Task.h"
#include "Msg.h"
#include "MsgService.h"
#include "RiverData.h"
#include "ArduinoJson.h"

class SerialCommTask : public Task{

    RiverData* riverData;
    StaticJsonDocument<100> receivedJson;
    Msg* btMsg;
    Msg* serialMsg;

    enum State {
        C0,
        C1,
        C2,
        C3
    };
    State state;

    public:
        SerialCommTask(Msg* btMsg, Msg* serialMsg, RiverData* riverData);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
    private:
        void saveDataAndSetMsgReady(Msg* msg);
};

#endif
