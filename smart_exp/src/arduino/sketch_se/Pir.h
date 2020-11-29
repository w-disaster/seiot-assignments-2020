#ifndef __PIR__
#define __PIR__

class Pir{
    public:
        virtual bool isHigh() = 0;
        virtual void interrupt() = 0;
        virtual void noInterrupt() = 0;
        static void motionDetected(){};
};

#endif
