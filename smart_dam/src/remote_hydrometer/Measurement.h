#ifndef __MEASUREMENT__
#define __MEASUREMENT__

class Measurement {
    long timestamp;
    float distance;

public:
    virtual void setTimestamp(long timestamp){
        this->timestamp = timestamp;
    }

    virtual void setDistance(float distance){
        this->distance = distance;
    }

    virtual long getTimestamp() { 
      return this->timestamp; 
    }

    virtual float getDistance() {
        return this->distance;
    }
};

#endif