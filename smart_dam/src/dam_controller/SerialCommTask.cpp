#include "Arduino.h"
#include "SerialCommTask.h"

SerialCommTask::SerialCommTask(RiverData* riverData, MsgServiceBT* btService){
    this->riverData = riverData;
    this->btService = btService;
}

void SerialCommTask::init(int period){
    Task::init(period);
}

bool SerialCommTask::updateTimeAndCheckEvent(int basePeriod){
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

void SerialCommTask::tick(){  
    switch(this->state){
        case C0:
            break;
        case C1:
            Msg *msg = MsgService.receiveMsg();

            DeserializationError error = deserializeJson(this->receivedJson, msg->getContent());

            if (!error) {   
                // we write river data coming from Dam Service to a shared object
                this->riverData->setRiverState(receivedJson["State"]);
                this->riverData->setDamMode(receivedJson["Mode"]);
                this->riverData->setDistance(receivedJson["Distance"]);
                
                digitalWrite(2, HIGH);
                delay(200);
                digitalWrite(2, LOW);
            }
            delete msg;
            break;
    }
}
