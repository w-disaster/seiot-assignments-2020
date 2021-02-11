#ifndef __RIVER_STATE__
#define __RIVER_STATE__

class RiverState{

public:
    enum State {
        NORMAL,
        PRE_ALARM,
        ALARM
    };
    State state;

    virtual void setState(State riverState) {
        state = riverState;
    }

    virtual State getState() {
        return state;
    }
};

#endif