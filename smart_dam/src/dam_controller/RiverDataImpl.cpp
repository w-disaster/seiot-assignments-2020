#include "RiverDataImpl.h"
#include "Arduino.h"

RiverDataImpl::RiverDataImpl(RiverData::RiverState initialState)
{
    setState(initialState);
    setMode(AUTOMATIC);
}

void RiverDataImpl::setState(RiverData::RiverState state)
{
    RiverData::setState(state);
}

RiverData::RiverState RiverDataImpl::getState()
{
    return RiverData::getState();
}

void RiverDataImpl::setMode(RiverData::Mode mode)
{
    RiverData::setMode(mode);
}

RiverData::Mode RiverDataImpl::getMode()
{
    return RiverData::getMode();
}

void RiverDataImpl::setDistance(float d){
    RiverData::setDistance(d);
}

float RiverDataImpl::getDistance(){
    return RiverData::getDistance();
}

void RiverDataImpl::setTimestamp(long t){
    RiverData::setTimestamp(t);
}

long RiverDataImpl::getTimestamp(){
    return RiverData::getTimestamp();
}

void RiverDataImpl::setMsgReady(bool flag){
    RiverData::setMsgReady(flag);
}
    
bool RiverDataImpl::isMsgReady(){
    return RiverData::isMsgReady();
}