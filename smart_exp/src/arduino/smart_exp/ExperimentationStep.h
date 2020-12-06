#ifndef __EXPERIMENTATION_STEP__
#define __EXPERIMENTATION_STEP__

class ExperimentationStep {

public:
    enum Step {
        READY,
        SUSPENDED,
        EXPERIMENTATION,
        EXPERIMENTATION_CONCLUDED,
        ERROR
    };
    Step step;

    virtual void setStep(Step expStep) {
        step = expStep;
    }

    virtual Step getStep() {
        return step;
    }
};

#endif
