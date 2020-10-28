#include <Arduino.h>
#include "gameLib.h"

void startGame(){
  if(canStart){
    // We turn red led off
    analogWrite(RED_LED, 0);
    Serial.println("Go!");
    isPlaying = true;
    alreadyOver = false;
    
    // We generate a random offset for the first led
    pinOffset = random(BUTTON_PIN_MAX - BUTTON_PIN_MIN + 1);

    /*
     * The maximum period time provided by MiniTimerOne is 4.194304s, therefore,
     * since the random() upper bound range is multiplied by K, we must divide it by K,
     * in order to give the change to not exceed the maximum value.
     * We subctract 1 from this value because at the beginning the timer must stand for
     * minimum 1 second.
     * 
     */
    int potValue = analogRead(POT);
    tMin = (4.194304/K - map(potValue, 0, 1023, 0, 4.194304/K - 1));
    tMin = tMin * MICROS_TO_SECONDS;
    
    long rndTimer = random(tMin, tMin * K);
    Serial.println(String("pot value : ") + potValue + String(" timer: ") + rndTimer);
    // We set the random period
    MiniTimer1.setPeriod(rndTimer);
    // We turn on the first led
    digitalWrite(LED_PIN_MIN + pinOffset, HIGH);
    // The timer starting
    MiniTimer1.start();
  }
}

void gameOver(){
  if(!alreadyOver){
    // We stop the timer and we reset it to the beginning
    MiniTimer1.stop();
    MiniTimer1.reset();

    // We turn off the current led on
    digitalWrite(LED_PIN_MIN + pinOffset, LOW);

    // We print the score
    Serial.println(String("Game Over! Final score: ") + score);

    // We reset the default global variables values 
    setGlobalVariables(false, 255, -1, 0, 0, true, false, 0, true);
    
  }
}
