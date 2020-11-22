#ifndef __TIMER_H__
#define __TIMER_H__

class Timer {
    public:  
        Timer();
        void setupFreq(int freq);  
        void setupPeriod(int period);  
        void waitForNextTick();
};

#endif
