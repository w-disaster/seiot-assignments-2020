#ifndef __DAM_SERVICE_COMM_TASK__
#define __DAM_SERVICE_COMM_TASK__

#include "Task.h"
#include "MsgService.h"
#include "ArduinoJson.h"


class DamServiceCommTask : public Task{

    StaticJsonDocument<100> receivedJson;
    
    public:
        DamServiceCommTask();
        void init(int period);
        bool updateTimeAndCheckEvent(int basePeriod);
        void tick();
};

#endif
