#include "Arduino.h"
#include "DamServiceCommTask.h"

DamServiceCommTask::DamServiceCommTask(){
    MsgService.init();
}

void DamServiceCommTask::init(int period){
    Task::init(period);
}

bool DamServiceCommTask::updateTimeAndCheckEvent(int basePeriod){
    return true;
}

void DamServiceCommTask::tick(){
    //!TEST
    if (MsgService.isMsgAvailable()) {
        Msg *msg = MsgService.receiveMsg();
        
        DeserializationError error = deserializeJson(receivedJson, msg->getContent());

        // Test if parsing succeeds.
        if (!error) {   
            
            digitalWrite(2, HIGH);
            delay(1000);
            digitalWrite(2, LOW);
        }
        
        
        /* NOT TO FORGET: message deallocation */
        delete msg;
    }
}
