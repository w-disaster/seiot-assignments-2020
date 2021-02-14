#include "RiverDataImpl.h"
#include "Arduino.h"

RiverDataImpl::RiverDataImpl(RiverDataImpl::RiverState initialState)
{
    getState(initialState);
    setMode(AUTOMATIC);
}

void RiverDataImpl::setState(RiverDataImpl::RiverState state)
{
    RiverData::setState(state);
}

RiverDataImpl::Step RiverDataImpl::getState()
{
    return RiverData::getState();
}

void RiverDataImpl::setMode(RiverDataImpl::Mode mode)
{
    RiverData::setMode(mode);
}

RiverDataImpl::Mode RiverDataImpl::getMode()
{
    return RiverData::getState();
}