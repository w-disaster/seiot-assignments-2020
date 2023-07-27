#include "Led.h"
#include "Arduino.h"

LedClass Led(LED_PIN);

LedClass::LedClass(int pin){
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void LedClass::switchOn(){
  digitalWrite(pin, HIGH);
  isOn = true;
}

void LedClass::switchOff(){
  digitalWrite(pin, LOW);
  isOn = false;
}

bool LedClass::isLedOn(){
  return isOn;
}

void LedClass::blink(){
  Led.isLedOn() ? Led.switchOff() : Led.switchOn();
}
