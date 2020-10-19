#include <Arduino.h>
#include <TimerOne.h>

#include "game_lib.h"

void startGame(){
  
    /* turn red led off */
    analogWrite(RED_LED, 0);
    Serial.print("Go!");
    isPlaying = true;

    
    /* pulsante random tra il 2,3,4,5 */
    flyPosition = random(BUTTON_MAX - BUTTON_MIN + 1);

    /* 
     * maps the diffuclty level  
     * lvl 1 = 8 seconds
     * lvl 8 = 1 second
     */
    tMin = (8 - map(analogRead(POT), 0, 1023, 1, 8)) * MILLIS_TO_SECONDS;
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
  /* spengo il led acceso appena perdo */
    digitalWrite(LED_MIN + flyPosition, LOW);
    
    Serial.println("GAME OVER!");
    
    /* reset default values */
    brightness = 255;
    verse = -1;
    isPlaying = false;

    /* prints the score then resets it */
    Serial.print("Final score: ");
    Serial.println(score);
    score = 0;

    analogWrite(RED_LED, brightness);
    delay(2*MILLIS_TO_SECONDS);
}
