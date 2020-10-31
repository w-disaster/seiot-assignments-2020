#include "gameLib.h"
#include <Arduino.h>

long tMin; 
int brightness;
int verse;
int pinOffset;
int score;
bool isPlaying;
bool missedLed;
bool canStart;
bool alreadyOver;
volatile unsigned long lastMicros;
bool pressed;

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
        
    /* We set the random period */
    MiniTimer1.setPeriod(random(tMin, tMin * K));
    /* Message printing */
    Serial.println("Go!");
    /* We turn on the first led */
    digitalWrite(LED_PIN_MIN + pinOffset, HIGH);
    /* The timer startGameing */
    MiniTimer1.start();
  }
}

void timesUp(){
  /* We stop the timer and we reset it to the beginning */
  MiniTimer1.stop();
  MiniTimer1.reset();
  if(pressed){ /* The current button was pressed in time */
      /* We turn off the current led */
      digitalWrite(LED_PIN_MIN + pinOffset, LOW);
      /* Next offset calculation */
      nextPinOffset();
      /* Turning on the next led */
      digitalWrite(LED_PIN_MIN + pinOffset, HIGH);

      /* Score increment, reset pressed variable */
      score++; 
      pressed = false;
      
      /* We reduce tMin in order to reduce the range whom the next expiring time will be calculated */
      tMin = tMin * REDUCING_FACTOR;
      /* We set the new period of the timer and we start it from the beginning */
      MiniTimer1.setPeriod(random(tMin, tMin * K));
      MiniTimer1.start();
  }
  /* 
   * Else, game over: current button not pressed in time / wrong button pressed
   * Checking this variable means be sure that this function is called only by 
   * buttonPressed() or by MiniTimerOne, avoiding a chain of interrupts   
   */
  else if(!alreadyOver){
    /* We turn off the current led on */
    digitalWrite(LED_PIN_MIN + pinOffset, LOW);

    /* We print the score */
    Serial.println(String("Game Over - Score: ") + score);

    /* We reset the default global variables values */
    setGlobalVariables(false, 255, -1, 0, 0, true, false, 0, true, false);
  }
}
