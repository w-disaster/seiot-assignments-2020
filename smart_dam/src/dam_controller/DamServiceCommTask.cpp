#include "Arduino.h"
#include "DamServiceCommTask.h"

DamServiceCommTask::DamServiceCommTask(RiverData* riverData){
    this->riverData = riverData;
}

void DamServiceCommTask::init(int period){
    MsgService.init();
    Task::init(period);
}

bool DamServiceCommTask::updateTimeAndCheckEvent(int basePeriod){
    if(Task::updateAndCheckTime(basePeriod)){
        if(MsgService.isMsgAvailable()){
            this->state = C1;
        }else {
            this->state = C0;
        }
        return true;
    }
    return false;
}

void DamServiceCommTask::tick(){  
    switch(this->state){
        case C0:
            break;
        case C1:
            Msg *msg = MsgService.receiveMsg();

            DeserializationError error = deserializeJson(receivedJson, msg->getContent());

            // Test if parsing succeeds.
            if (!error) {   
                
                this->riverData->setState(receivedJson["State"]);
                this->riverData->setDistance(receivedJson["Distance"]);
                this->riverData->setTimestamp(receivedJson["Timestamp"]);
                this->riverData->setMsgReady(true);

                digitalWrite(2, HIGH);
                delay(200);
                digitalWrite(2, LOW);
            }
            /* NOT TO FORGET: message deallocation */
            delete msg;
            break;
    }
}
