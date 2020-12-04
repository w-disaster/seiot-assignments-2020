#ifndef __KINEMATICS_DATA_IMPL__
#define __KINEMATICS_DATA_IMPL__

#include "KinematicsData.h"
       
class KinematicsDataImpl : public KinematicsData{
    public:
        KinematicsDataImpl();
        void setDistance(float x);
        float getDistance();
        void setSpeed(float v);
        float getSpeed();
        void setAcceleration(float a);
        float getAcceleration();
        void setDataReady(bool flag);
        bool isDataReady();
};

#endif