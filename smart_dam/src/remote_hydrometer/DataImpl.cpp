#include "Arduino.h"
#include "DataImpl.h"

DataImpl::DataImpl(String addr){
  
}

void DataImpl::setDistance(float d){
  Data::setDistance(d);
}

float DataImpl::getDistance(){
  return Data::getDistance();
}
