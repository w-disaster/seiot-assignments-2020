#ifndef __VIEWER_COMM__
#define __VIEWER_COMM__

#include "Task.h"
#include "Arduino.h"

class ViewerComunication : public Task
{

public:
    /**
     * Method to get some information to the ViewerComunicationTask 
     */
    virtual void getData(double[] data) = 0;
}

#endif