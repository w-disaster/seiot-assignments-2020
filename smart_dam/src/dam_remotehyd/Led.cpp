#include "Led.h"
#include "Arduino.h"

LedClass* Led = new LedClass(LED_PIN);

LedClass::LedClass(int pin){
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void LedClass::switchOn(){
  digitalWrite(pin, HIGH);
}

void LedClass::switchOff(){
  digitalWrite(pin, LOW);
}