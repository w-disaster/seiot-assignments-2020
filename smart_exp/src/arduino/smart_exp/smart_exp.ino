#include "smart_exp.h"

Button* buttonStart;
Button* buttonStop;

void setup(){

  buttonStart = new ButtonStart(BUTTON_START_PIN);
  buttonStop = new ButtonStop(BUTTON_STOP_PIN);
  
  Serial.begin(9600);
}

void loop(){
  
}
