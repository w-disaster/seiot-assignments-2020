#include "LedImpl.h"
#include "Arduino.h"

LedImpl::LedImpl(int pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void LedImpl::turnOn()
{
  digitalWrite(this->pin, HIGH);
}

void LedImpl::turnOff()
{
  digitalWrite(this->pin, LOW);
}

void LedImpl::blinkLed(int milliseconds, int blinkStep)
{
  /* fìrst the led turns ON */
  bool ledIsOn = false;

  while (blinkStep < milliseconds)
  {

    ledIsOn ? turnOff() : turnOn();

    delay(blinkStep);
    milliseconds -= blinkStep;
  }

  /* if some milliseconds have remained */
  if (milliseconds)
    delay(milliseconds);

  /* at the end we turn OFF the led*/
  turnOff();
}
