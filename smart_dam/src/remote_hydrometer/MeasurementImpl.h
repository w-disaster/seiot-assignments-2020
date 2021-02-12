#ifndef __MEASUREMENT_IMPL__
#define __MEASUREMENT_IMPL__

#include "Measurement.h"

class MeasurementImpl : public Measurement {
    public:
        void setTimestamp(long t);
        void setDistance(float d);
        long getTimestamp();
        float getDistance();
};

#endif