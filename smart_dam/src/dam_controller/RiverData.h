#ifndef __RIVER_DATA__
#define __RIVER_DATA__

class RiverData {

public:
    enum RiverState{
        NORMAL,
        PREALARM,
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

    void setDamOpening(int damOpening);

    int getDamOpening();

private:
    RiverState riverState;
    DamMode damMode;
    int damOpening;

};

#endif
