#include "PirImpl.h"
#include "Arduino.h"
#include "EnableInterrupt.h"

#define CALIBRATION_TIME 10

PirImpl::PirImpl(int pin){
    this->pin = pin;
    pinMode(this->pin, INPUT);

    Serial.print("Calibrating PIR ");
    for (int i = 0; i < CALIBRATION_TIME; i++)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("rotto i coglioniiii");
}

bool PirImpl::isHigh(){
    return digitalRead(this->pin);
}

void PirImpl::interrupt(){
    enableInterrupt(this->pin, Pir::motionDetected, RISING);
}

void PirImpl::noInterrupt(){
    disableInterrupt(this->pin);
}

