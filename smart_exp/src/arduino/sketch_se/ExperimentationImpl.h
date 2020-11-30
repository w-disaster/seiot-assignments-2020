#ifndef __EXPERIMENTATION_IMPL__
#define __EXPERIMENTATION_IMPL__

#include "Experimentation.h"

class ExperimentationImpl : public Experimentation{
    public:
        ExperimentationImpl(State initialState);
        void setExperimentationState(State expState);
        State getExperimentationState();
        void setDistance(float x);
        float getDistance();
        void setSpeed(float v);
        float getSpeed();
        void setAcceleration(float a);
        float getAcceleration();
};

#endif