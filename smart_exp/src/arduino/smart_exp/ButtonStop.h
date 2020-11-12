#ifndef __BUTTON_STOP__
#define __BUTTON_STOP__

#include "Button.h"

class ButtonStop : public Button
{

public:
    ButtonStop(int pin);
    void pressButton();

private:
    int pin;
};

#endif
