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

        virtual void setDistance(float x){
            distance = x;
        }

        virtual float getDistance(){
            return distance;
        }

        virtual void setSpeed(float v){
            speed = v;
        }

        virtual float getSpeed(){
            return speed;
        }

        virtual void setAcceleration(float a){
            acceleration = a;
        }

        virtual float getAcceleration(){
            return acceleration;
        }

    private:
        State state;
        float distance;
        float speed;
        float acceleration;
        
};

#endif
