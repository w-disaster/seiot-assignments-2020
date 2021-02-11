#ifndef __RIVER_STATE__
#define __RIVER_STATE__

#include "RiverState.h"

class RiverStateImpl : public RiverState{
    public:
        virtual void setState(State riverState);
        virtual State getState();
};

#endif