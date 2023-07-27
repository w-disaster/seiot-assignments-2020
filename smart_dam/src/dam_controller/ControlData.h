#ifndef __CONTROL_DATA__
#define __CONTROL_DATA__

#include "ArduinoJson.h"
#define C_JSON_DIM 20

/* This class is useful to fetch data of a json sent from DM */
class ControlData {
    public:
        ControlData(String msg);
        boolean containsDamOpening();
        int getDamOpening();

    private:
        DeserializationError error;

};

#endif