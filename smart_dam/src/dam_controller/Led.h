#ifndef __LED__
#define __LED__

#include "Light.h"
#define LED_PIN 4

/* Led class */
class LedClass: public Light { 
  public:
    LedClass(int pin);
    void switchOn();
    void switchOff();
    bool isLedOn();
    static void blink();

    int pin;
    enum { ON, OFF } state;  
    bool isOn;
};

extern LedClass Led;

#endif
