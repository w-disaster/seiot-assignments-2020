#ifndef __LED_IMPL__
#define __LED_IMPL__

#include "Led.h"

class LedImpl: public Led
{

public:
  LedImpl(int pin);
  void turnOn();
  void turnOff();
  void blinkLed(int seconds);
  
private:
  int pin;
};
#endif
