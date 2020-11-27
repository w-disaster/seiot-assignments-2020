#include "Experimentation.h"

class ExperimentationImpl : public Experimentation{
    public:
        ExperimentationImpl(State initialState);
        void setExperimentationState(State expState);
        State getExperimentationState();
};