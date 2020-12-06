#ifndef __EXPERIMENTATION_STEP_IMPL__
#define __EXPERIMENTATION_STEP_IMPL__

#include "ExperimentationStep.h"

class ExperimentationStepImpl : public ExperimentationStep{
    public:
        ExperimentationStepImpl(Step initialStep);
        void setStep(Step expStep);
        Step getStep();
};

#endif