#include <Arduino.h>
#include "gameLib.h"

void startGame(){
  if(canStart){
    /* We turn red led off */
    analogWrite(RED_LED_PIN, 0);
    isPlaying = true;
    alreadyOver = false;
    
    /* We generate a random offset for the first led */
    pinOffset = random(BUTTON_PIN_MAX - BUTTON_PIN_MIN + 1);

    /*
     * The maximum period time provided by MiniTimerOne is 4.194304s, therefore,
     * since the random() upper bound range is multiplied by K, we must divide it by K,
     * in order to give the change to not exceed the maximum value.
     */
    int potValue = analogRead(POT);
    int level = map(potValue, 0, 1023, 1, 8);
    tMin = (MTO_MAX_PERIOD - map(level, 1, 8, 0, MTO_MAX_PERIOD - MICROS_TO_SECONDS)) / K;
        
    long rndTimer = random(tMin, tMin * K);
    /* We set the random period */
    MiniTimer1.setPeriod(rndTimer);
    /* We turn on the first led */
    digitalWrite(LED_PIN_MIN + pinOffset, HIGH);
    /* The timer startGameing */
    MiniTimer1.start();
  }
}

void timesUp(){
  if(pressed){
      /* We stop the timer, the button was pressed in time */
      MiniTimer1.stop();
      // We turn off the current led and we turn on the next one
      digitalWrite(LED_PIN_MIN + pinOffset, LOW);
      /* Score increment */
      score++; 
      pressed = false;
      Serial.println(String("Tracking the fly: pos ") + (BUTTON_PIN_MIN + pinOffset));
      /* Next offset calculation */
      nextPinOffset();
      digitalWrite(LED_PIN_MIN + pinOffset, HIGH);
      /* We reduce tMin in order to reduce the range whom the next expiring time will be calculated */
      tMin = tMin * REDUCING_FACTOR;
      /* We set the new period of the timer and we startGame it from the beginning */
      MiniTimer1.setPeriod(random(tMin, tMin * K));
      MiniTimer1.reset();
      MiniTimer1.start();
  }
  /* 
   * checking this variable means be sure that this function is called only by 
   * buttonPressed() or by MiniTimerOne, avoiding a chain of interrupts   
   */
  else if(!alreadyOver){
    /* We stop the timer and we reset it to the beginning */
    MiniTimer1.stop();
    MiniTimer1.reset();

    /* We turn off the current led on */
    digitalWrite(LED_PIN_MIN + pinOffset, LOW);

    /* We print the score */
    Serial.println(String("Game Over - Score: ") + score);

    /* We reset the default global variables values */
    setGlobalVariables(false, 255, -1, 0, 0, true, false, 0, true, false);
  }
}