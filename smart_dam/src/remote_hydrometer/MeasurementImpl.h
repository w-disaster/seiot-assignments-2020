#ifndef __MEASUREMENT_IMPL__
#define __MEASUREMENT_IMPL__

#include "Measurement.h"

class MeasurementImpl : public Measurement {
    public:
        void setTimestamp(long timestamp);
        void setDistance(float distance);
        long getTimestamp();
        float getDistance();
};

#endif