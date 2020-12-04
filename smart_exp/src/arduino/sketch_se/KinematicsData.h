#ifndef __KINEMATICS_DATA__
#define __KINEMATICS_DATA__

class KinematicsData{

    float distance;
    float speed;
    float acceleration;
    bool dataReady;

    public:
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

        virtual void setDataReady(bool flag){
            dataReady = flag;
        }

        virtual bool isDataReady(){
            return dataReady;
        }

};


#endif