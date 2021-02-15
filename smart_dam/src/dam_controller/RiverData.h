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

    float distance;
    long timestamp;
    bool msgReady;

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

    virtual void setDistance(float d){
        distance = d;
    }

    virtual float getDistance(){
        return distance;
    }

    virtual void setTimestamp(long t){
        timestamp = t;
    }
    
    virtual long getTimestamp(){
        return timestamp;
    }

    virtual void setMsgReady(bool flag){
        msgReady = flag;
    }
    
    virtual bool isMsgReady(){
        return msgReady;
    }
};

#endif
