#include "ButtonStart.h"
#include "Arduino.h"

ButtonStart::ButtonStart(int pin)
{
  this->pin = pin; //NOTA: private pin potrebbe essere ridondante/inutile.
  pinMode(pin, INPUT);
}

void ButtonStart::pressButton()
{
  //TODO: implement button mechanics
}
