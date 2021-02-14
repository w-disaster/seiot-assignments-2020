#ifndef __RIVER_DATA__
#define __RIVER_DATA__

class RiverData
{

public:
    enum RiverState
    {
        NORMAL,
        PRE_ALARM,
        ALARM
    };
    RiverState riverState;

    enum Mode
    {
        MANUAL,
        AUTOMATIC
    };
    Mode mode;

    virtual void setState(RiverState state)
    {
        riverState = state;
    }

    virtual RiverState getState()
    {
        return riverState;
    }

    virtual void setMode(Mode controlMode)
    {
        mode = controlMode;
    }

    virtual Mode getMode()
    {
        return mode;
    }
};

#endif
