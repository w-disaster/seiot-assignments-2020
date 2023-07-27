#ifndef __RIVER_DATA__
#define __RIVER_DATA__

#include "ArduinoJson.h"
#include "State.h"
#define D_JSON_DIM 100

/* This class is useful to fetch data of a json sent from DS */
class RiverData {
    public:
        RiverData(String msg);
        State getRiverState();
        bool containsDamOpening();
        int getDamOpening();

    private:
        DeserializationError error;

};

#endif