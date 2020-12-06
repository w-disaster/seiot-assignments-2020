#ifndef __PIR_IMPL__
#define __PIR_IMPL__

#include "Pir.h"

class PirImpl : public Pir{
    int pin;
    public:
        PirImpl(int pin);
        bool isHigh();
        void interrupt();
        void noInterrupt();
};

#endif
