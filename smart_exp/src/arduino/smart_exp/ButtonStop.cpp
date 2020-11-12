#include "ButtonStop.h"
#include "Arduino.h"

ButtonStop::ButtonStop(int pin)
{
  this->pin = pin; //NOTA: private pin potrebbe essere ridondante/inutile.
  pinMode(pin, INPUT);
}

void ButtonStop::pressButton()
{
  //TODO: implement button mechanics
}
