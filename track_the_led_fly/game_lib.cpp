#include <Arduino.h>
#include "game_lib.h"

void startGame(){
  
}

void moveFly(){
  noInterrupts();
  
  int nextStep = random(0,1);
  /* muovo la fly a destra o sinistra randoimicamente */
  nextStep ? flyPosition++ : flyPosition--;

  /* overflow control */
  if(flyPosition > BUTTON_MAX-BUTTON_MIN) flyPosition = 0;
  if(flyPosition < 0) flyPosition = BUTTON_MAX-BUTTON_MIN;

 interrupts(); 
}

void gameOver(){
  
}
