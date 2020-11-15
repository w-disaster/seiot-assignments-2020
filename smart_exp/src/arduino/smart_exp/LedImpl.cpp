#include "LedImpl.h"
#include "Arduino.h"

#define BLINK_DELAY

LedImpl::LedImpl(int pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void LedImpl::turnOn() {
  digitalWrite(this->pin, HIGH);
}


void LedImpl::turnOff() {
  digitalWrite(this->pin, LOW);
}

void LedImpl::blinkLed(int seconds) {
  
}
