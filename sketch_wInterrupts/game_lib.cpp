#include <Arduino.h>
#include "game_lib.h"

void startGame(){
  if(canStart){
    /* turn red led off */
    analogWrite(RED_LED, 0);
    //Serial.println("Go!");
    isPlaying = true;

    /* pulsante random tra il 2,3,4,5 */
    pinOffset = random(BUTTON_MAX - BUTTON_MIN + 1);

    /* 
     * maps the diffuclty level  
     * lvl 1 = 8 seconds
     * lvl 8 = 1 second
     */
    tMin = (8 - map(analogRead(POT), 0, 1023, 0, 7));
    tMin = tMin * MICROS_TO_SECONDS;
    Serial.println(String("tmin : ") + tMin);

    long rndTimer = random(tMin, tMin * K);
    Serial.println(String("timer: ") + rndTimer);
    MiniTimer1.setPeriod(rndTimer);
    
    MiniTimer1.attachInterrupt(gameOver);
    digitalWrite(LED_MIN + pinOffset, HIGH);
    MiniTimer1.start();
  }
}


void gameOver(){
    MiniTimer1.stop();
    /* spengo il led acceso appena perdo */
    digitalWrite(LED_MIN + pinOffset, LOW);
        
    /* reset default values */
    brightness = 255;
    verse = -1;
    isPlaying = false;
    pinOffset = 0;
    missedLed = true;
    
    /* prints the score then resets it */
    Serial.println(String("Game Over! Final score: ") + score);
    score = 0;
}
