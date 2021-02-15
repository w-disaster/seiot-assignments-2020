#ifndef __DAM_SERVICE_COMM_TASK__
#define __DAM_SERVICE_COMM_TASK__

#include "Task.h"
#include "MsgService.h"
#include "ArduinoJson.h"
#include "RiverData.h"


class DamServiceCommTask : public Task{

    StaticJsonDocument<100> receivedJson;
    RiverData* riverData;

    enum State {
        C0,
        C1
    };
    State state;

    public:
        DamServiceCommTask(RiverData* riverData);
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};

#endif
