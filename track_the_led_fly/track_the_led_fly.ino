/*
 * PARTECIPANTI DEL GRUPPO
 * - Luigi Olivieri
 * - Luca Fabri
 */
 
#define EI_ARDUINO_INTERRUPTED_PIN
#include "gameLib.h"
#include <EnableInterrupt.h>

void setup() {
  /* Global variables set */
  setGlobalVariables(false, 255, -1, 0, 0, false, true, 0, false, false);

  /* Led and buttons setup */
  pinMode(RED_LED_PIN, OUTPUT);
  for(int i = 0; i <= LED_PIN_MAX - LED_PIN_MIN; i++){
    pinMode(LED_PIN_MIN + i, OUTPUT);
    pinMode(BUTTON_PIN_MIN + i, INPUT);
    enableInterrupt(BUTTON_PIN_MIN + i, buttonPressed, RISING);
  }

  /* Pseudo number generator init in unused pin */
  randomSeed(analogRead(A1));

  Serial.begin(115200);
  Serial.println("Welcome to the Track to Led Fly Game. Press Key T1 to Start");

  /*
   * timer setup: at times up timesUp will be called. We don't start it
   * because the player didn't press T1 yet.
   */
  MiniTimer1.init();
  MiniTimer1.attachInterrupt(timesUp);
}

void loop() {
  /* If the player is not playing and timesUp was not called */
  if(!isPlaying && !missedLed){
    noInterrupts();
    fadeRedLed();
    interrupts();
  } else if(missedLed){ /* timesUp() sets true this global variable at game over */
    /* When the red led go HIGH for two seconds, the game cannot start at T1 pressing */
    noInterrupts();
    canStart = false;
    interrupts();
    /* Set at HIGH for 2 seconds the red led */
    analogWrite(RED_LED_PIN, brightness);
    delay(2 * MILLIS_TO_SECONDS);
    missedLed = false;
    /* Now the player can replay the game */
    noInterrupts();
    canStart = true;
    interrupts();
  }
  delay(10);
}

void buttonPressed(){
  /* Debouncing: 200'000micros = 200ms between two interrupts */
  if(micros() - lastMicros > 200000) {    
    /* The game didn't started and T1 is pressed then game must start */
    if(!isPlaying){
      if(arduinoInterruptedPin == BUTTON_PIN_MIN){
        startGame();
      }
    }
    /* 
     * Right button pressed:
     * when the time is up, setting pressed = true, the game will not be over
     */
    else if(isPlaying && arduinoInterruptedPin == BUTTON_PIN_MIN + pinOffset){
      if(!pressed){
        Serial.println(String("Tracking the fly: pos ") + (BUTTON_PIN_MIN + pinOffset));
        pressed = true;
      }
    }
    /* In all the other cases the player pressed a wrong button, then timesUp will be called */
    else if(isPlaying) {
      timesUp();
    }
    lastMicros = micros();
  }
}

void fadeRedLed(){
    analogWrite(RED_LED_PIN, brightness);
    brightness = brightness + verse * FADE_STEP;
    /* Change direction when lower/upper bound reached */
    if(brightness == 255 || brightness == 0){
      verse = -(verse);
    }
}

void nextPinOffset(){
  int next;
  /* We move the fly in an adjacent position and then we manage the overflow state */
  next = random(0,2) ? pinOffset + 1  : pinOffset - 1;
  /* If the next offsed exeeds the maximum range then it's set at minimum (0) */
  if(next > BUTTON_PIN_MAX - BUTTON_PIN_MIN) next = 0;
  /* If the offsed is negative, the maximum must be set */
  if(next < 0) next = BUTTON_PIN_MAX - BUTTON_PIN_MIN;
  
  pinOffset = next;
}

void setGlobalVariables(bool pIsPlaying, int pBrightness, int pVerse, int pPinOffset, int pScore, \
  bool pMissedLed, bool pCanStart, unsigned long pLastMicros, bool pAlreadyOver, bool pPressed) {
  score = pScore;
  isPlaying = pIsPlaying;
  brightness = pBrightness;
  verse = pVerse;
  pinOffset = pPinOffset;
  missedLed = pMissedLed;
  canStart = pCanStart;
  lastMicros = pLastMicros;
  alreadyOver = pAlreadyOver;
  pressed = pPressed;
}
