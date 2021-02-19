#ifndef __DAM_SERVICE_COMM_TASK__
#define __DAM_SERVICE_COMM_TASK__

#include "Task.h"
#include "MsgServiceClass.h"
#include "MsgServiceBT.h"
#include "RiverData.h"
#include "ArduinoJson.h"

class SerialCommTask : public Task{

    MsgServiceBT* btService;
    RiverData* riverData;
    StaticJsonDocument<100> receivedJson;

    enum State {
        C0,
        C1
    };
    State state;

    public:
        SerialCommTask(RiverData* riverData, MsgServiceBT* btService);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};

#endif
