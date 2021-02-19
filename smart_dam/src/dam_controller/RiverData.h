#ifndef __RIVER_DATA__
#define __RIVER_DATA__

class RiverData {

public:
    enum RiverState{
        NORMAL,
        PRE_ALARM,
        ALARM
    };
    RiverState riverState;

    enum DamMode{
        AUTO,
        MANUAL
    };

    void setRiverState(RiverState riverState);

    RiverState getRiverState();

    void setDamMode(DamMode damMode);

    DamMode getDamMode();

    void setDistance(float distance);

    float getDistance();

private:
    DamMode damMode;
    float distance;

};

#endif
