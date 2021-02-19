#include "Arduino.h"
#include "BTCommTask.h"

BTCommTask::BTCommTask(Msg* serialMsg, Msg* btMsg){
    this->btService = new MsgServiceBT(RX, TX);
    this->serialMsg = serialMsg;
    this->btMsg = btMsg;
}

void BTCommTask::init(int period){
    Task::init(period);
    btService->init();
}

bool BTCommTask::updateTimeAndCheckEvent(int basePeriod){
    if(Task::updateAndCheckTime(basePeriod)){
        /* If a task (in this case SerialCommTask) tells us 
         *  to forward a message through bluetooth we do so
         */
        if(this->serialMsg->isMsgReady()){
            /* If also a message arrived from bluethoot at tick()
             *  we execute both operations
             */
            if(this->btService->isMsgAvailable()){
                this->state = BT3;
            }else {
                this->state = BT1;
            }
            return true;
        }
        /* We can receive msg just from blueetooth */
        if(this->btService->isMsgAvailable()){
            this->state = BT2;
            return true;
        }
    }
    return false;
}

void BTCommTask::tick(){
    switch(this->state){
        case BT1:
            this->btService->sendMsg(*this->serialMsg);
            break;
        case BT2:
            this->btMsg = this->btService->receiveMsg();
            break;
        case BT3:
            this->btService->sendMsg(*this->serialMsg);
            this->btMsg = this->btService->receiveMsg();
            break;
    }
}
