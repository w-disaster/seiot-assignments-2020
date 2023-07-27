#ifndef __TASK__
#define __TASK__

class Task {
    public: 
        void init(int period);
        void attachInt();
        void detachInt();
        static void tick();
};

#endif
