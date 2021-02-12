#ifndef __RIVER_STATE_IMPL__
#define __RIVER_STATE_IMPL__

#include "RiverState.h"

class RiverStateImpl : public RiverState {
    public:
        void setState(RiverState::State riverState);
        RiverState::State getState();
};

#endif
