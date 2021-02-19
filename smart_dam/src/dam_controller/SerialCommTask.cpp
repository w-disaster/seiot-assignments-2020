#include "Arduino.h"
#include "SerialCommTask.h"

SerialCommTask::SerialCommTask(Msg* btMsg, Msg* serialMsg, RiverData* riverData){
    this->btMsg = btMsg;
    this->serialMsg = serialMsg;
    this->riverData = riverData;
}

void SerialCommTask::init(int period){
    Task::init(period);
}

bool SerialCommTask::updateTimeAndCheckEvent(int basePeriod){
    if(Task::updateAndCheckTime(basePeriod)){
        /* If a message is arrived from Serial */
        if(MsgService.isMsgAvailable()){
            /* If a message also is arrived from bluetooth */
            if(this->btMsg->isMsgReady()){
                this->state = C3;
            } else { /* the message arrives only from Serial */
                this->state = C1;
            }
            return true;
        }
        /* If a message only arrives from bluetooth */
        if(this->btMsg->isMsgReady()){
            this->state = C2;
            return true;
        }
    }
    return false;
}

void SerialCommTask::tick(){  
    switch(this->state){
        case C0:
            break;
        case C1:
            /* If a message arrives from Serial we save it in the RiverData
             *  shared object and we set the msg ready to be sent
             */
            saveDataAndSetMsgReady(MsgService.receiveMsg());
            break;
        case C2:
            /* If BTCommTask received a message from bluetooth we must
             *  forward it to Serial channel
             */
            MsgService.sendMsg(this->btMsg->getContent());
            this->btMsg->setMsgReady(false);
            break;
        case C3:
            /* We do both activities: we set the message ready
             *  to be sent through bluetooth and we forward 
             *  Dam Mobile msg Serial channel 
             */
            saveDataAndSetMsgReady(MsgService.receiveMsg());
            MsgService.sendMsg(this->btMsg->getContent());
            this->btMsg->setMsgReady(false);
            break;        
    }
}

/* This method minimize repetitions. It sets RiverData fields and the new message 
 *  to be sent from BTCommTask
 */
void SerialCommTask::saveDataAndSetMsgReady(Msg* msg){
    this->serialMsg = msg;
    DeserializationError error = deserializeJson(this->receivedJson, this->serialMsg->getContent());

    if (!error) { 
        // we write river data coming from Dam Service to a shared object
        String stateFromJson = this->receivedJson["State"];
        RiverData::RiverState riverState;
    
        /* Json file contains river state as a string, we must convert it to the
         *  specified enum value
         */
        if(stateFromJson == "NORMAL"){
            riverState = RiverData::RiverState::NORMAL;
        } else if(stateFromJson == "PRE_ALARM"){
            riverState = RiverData::RiverState::PRE_ALARM;
        } else{
            riverState = RiverData::RiverState::ALARM;
        }
    
        /* Same for Dam Mode */
        String modeFromJson = this->receivedJson["Mode"];
        RiverData::DamMode damMode;
    
        if(modeFromJson == "AUTO"){
            damMode = RiverData::DamMode::AUTO;
        }else {
            damMode = RiverData::DamMode::MANUAL;
        }
    
        /* Shared object value set, ready to be read by ServoMotorTask and LedTask */
        this->riverData->setRiverState(riverState);
        this->riverData->setDamMode(damMode);
        this->riverData->setDistance(this->receivedJson["Distance"]);
      
        
        digitalWrite(4, HIGH);
        delay(200);
        digitalWrite(4, LOW);
    }
}
