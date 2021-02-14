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