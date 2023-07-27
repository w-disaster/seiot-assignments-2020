#ifndef __LED__
#define __LED__

#include "Light.h"
#define LED_PIN D2  

/* Led class */
class LedClass: public Light { 
  public:
    LedClass(int pin);
    void switchOn();
    void switchOff();

  private:
    int pin;
};

extern LedClass* Led;

#endif
