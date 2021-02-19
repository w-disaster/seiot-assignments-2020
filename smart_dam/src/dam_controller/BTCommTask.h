#ifndef __BT_COMM_TASK__
#define __BT_COMM_TASK__

class BTCommTask : public Task{

public:
    BTCommTask();
    void init(int period);
    bool updateTimeAndCheckEvent(int basePeriod);
    void tick();
}


#endif