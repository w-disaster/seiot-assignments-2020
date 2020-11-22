#ifndef __LIGHT_H__
#define __LIGHT_H__

class Light {
public:
  virtual void switchOn() = 0;
  virtual void switchOff() = 0;    
};

#endif
