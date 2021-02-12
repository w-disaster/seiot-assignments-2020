#ifndef __MEASUREMENT__
#define __MEASUREMENT__

class Measurement {
    long timestamp;
    float distance;

public:
    virtual void setTimestamp(long t){
        timestamp = t;
    }

    virtual void setDistance(float d){
        distance = d;
    }

    virtual long getTimestamp() { 
      return timestamp; 
    }

    virtual float getDistance() {
        return distance;
    }
};

#endif
