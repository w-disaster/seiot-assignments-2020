#include "RiverData.h"
#include "Arduino.h"

StaticJsonDocument<D_JSON_DIM> riverDataJson;

RiverData::RiverData(String msg){
    this->error = deserializeJson(riverDataJson, msg);
}

State RiverData::getRiverState(){
    State res;
    if(!error){
        String stateAsString = riverDataJson["S"];
        int stateCode = stateAsString.toInt();
        switch(stateCode){
            case 0:
                res = NORMAL;
                break;
            case 1:
                res = PRE_ALARM;
                break;
            case 2:
                res = ALARM;
                break;
       }
   }
   return res;
}

boolean RiverData::containsDamOpening(){
    JsonVariant error = riverDataJson["DO"];
    return !error.isNull();
}


int RiverData::getDamOpening(){
    return riverDataJson["DO"];
}