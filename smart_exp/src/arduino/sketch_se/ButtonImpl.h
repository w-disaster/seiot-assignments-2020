#ifndef __BUTTON_IMPL__
#define __BUTTON_IMPL__

#include "Button.h"

class ButtonImpl : public Button{
    int pin;
    public:
        ButtonImpl(int pin);
        bool isPressed();
};

#endif