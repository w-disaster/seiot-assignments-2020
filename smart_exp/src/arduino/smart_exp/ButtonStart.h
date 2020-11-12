#ifndef __BUTTON_START__
#define __BUTTON_START__

#include "Button.h"

class ButtonStart : public Button
{

public:
  ButtonStart(int pin);
  void pressButton();

private:
  int pin;
};

#endif
