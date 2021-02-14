#ifndef __RIVER_DATA_IMPL__
#define __RIVER_DATA_IMPL__

#include "RiverData.h"

class RiverDataImpl : public RiverData
{
public:
    RiverDataImpl(RiverState initialState);
    void setState(RiverState state);
    RiverState getState();
    void setMode(Mode controlMode);
    Mode getMode();
};

#endif