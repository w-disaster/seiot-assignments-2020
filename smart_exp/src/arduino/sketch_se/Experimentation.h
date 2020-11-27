#ifndef __EXPERIMENTATION__
#define __EXPERIMENTATION__

class Experimentation{
    public:
        enum State {READY, SUSPENDED, EXPERIMENTATION, EXPERIMENTATION_CONCLUDED, ERROR};
        
        virtual void setExperimentationState(State expState){
            state = expState;
        }
        
        virtual State getExperimentationState(){
            return state;
        }
    private:
        State state;
};

#endif
