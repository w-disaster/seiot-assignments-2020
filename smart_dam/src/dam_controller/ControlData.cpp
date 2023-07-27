#include "ControlData.h"
#include "Arduino.h"

StaticJsonDocument<C_JSON_DIM> controlDataJson;

ControlData::ControlData(String msg){
    this->error = deserializeJson(controlDataJson, msg);
}

boolean ControlData::containsDamOpening(){
    JsonVariant error = controlDataJson["D"];
    return !error.isNull();
}

int ControlData::getDamOpening(){
    return controlDataJson["D"];
}
