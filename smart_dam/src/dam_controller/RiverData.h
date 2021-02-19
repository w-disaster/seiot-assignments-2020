#ifndef __RIVER_DATA__
#define __RIVER_DATA__

class RiverData {

public:
    enum RiverState{
        NORMAL,
        PRE_ALARM,
        ALARM
    };

    enum DamMode{
        AUTO,
        MANUAL
    };

    RiverData();
    void setRiverState(RiverState riverState);

    RiverState getRiverState();

    void setDamMode(DamMode damMode);

    DamMode getDamMode();

    void setDistance(float distance);

    float getDistance();

private:
    RiverState riverState;
    DamMode damMode;
    float distance;

};

#endif
