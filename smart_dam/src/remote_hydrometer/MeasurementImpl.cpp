#include "MeasurementImpl.h"
#include "Arduino.h"

void setTimestamp(long timestamp){
    Measurement::setTimestamp(timestamp);
}

void setDistance(float distance){
    Measurement::setDistance(distance);
}

long getTimestamp() { 
    return Measurement::getTimestamp();
}

float getDistance() {
    return Measurement::getDistance();
}