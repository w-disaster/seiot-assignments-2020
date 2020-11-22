#ifndef __LED_H__
#define __LED_H__

#include "Light.h"

class Led: public Light { 
public:
  Led(int pin);
  void switchOn();
  void switchOff();    
private:
  int pin;  
};

#endif