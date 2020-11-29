#ifndef __PIR__
#define __PIR__

class Pir{
    public:
        virtual bool isHigh() = 0;
        virtual void enableInterrupt() = 0;
        virtual void disableInterrupt() = 0;
};

#endif
