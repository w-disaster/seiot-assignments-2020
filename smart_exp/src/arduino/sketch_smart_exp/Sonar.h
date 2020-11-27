#ifndef __SONAR__
#define __SONAR__

#define DHTTYPE DHT11


class Sonar{
    public:
        virtual float getDistance() = 0;
        
}

#endif