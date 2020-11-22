#ifndef __BUTTON_IMPL_H__
#define __BUTTON_IMPL_H__

#include "Button.h"

class ButtonImpl : public Button{
    int pin;
    public:
        ButtonImpl(int pin);
        bool isPressed();
};

#endif