#ifndef __DATA__
#define __DATA__

class Data {

  float distance;
  
  public:
    virtual void setDistance(float d){
      distance = d;
    }
    virtual float getDistance(){
      return distance;
    }
    static void send();
};

#endif
