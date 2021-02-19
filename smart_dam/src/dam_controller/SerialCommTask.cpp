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
            this->serialMsg = MsgService.receiveMsg();

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
            //delete msg;
            break;
    }
}
