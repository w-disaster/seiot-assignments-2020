/*
 * PARTECIPANTI DEL GRUPPO
 * - Luigi Olivieri
 * - Luca Fabri
 */
 
#define EI_ARDUINO_INTERRUPTED_PIN
#include "gameLib.h"
#include <EnableInterrupt.h>

// Variable for random time
double tMin; 
// Fading variables
int brightness;
int verse;

/* offset from: 
 *  - LED_PIN_MIN => offset + LED_PIN_MIN = current led at HIGH
 *  - BUTTON_PIN_MIN => offset + BUTTON_PIN_MIN = current button to press
 */
int pinOffset;
// Current score 
int score;
// State of the game
bool isPlaying;
/*
 * We set true this variable when the time is up or the player pressed a wrong
 * button: the red led must go HIGH for 2 seconds
 */
bool missedLed;
// the player can start or not
bool canStart;
bool alreadyOver;
// time in micros for button isr debounce
volatile unsigned long lastMicros;


void setup() {

  setGlobalVariables(false, 255, -1, 0, 0, false, true, 0, false);

  /* led and buttons setup */
  pinMode(RED_LED, OUTPUT);
  for(int i = 0; i <= LED_PIN_MAX - LED_PIN_MIN; i++){
    pinMode(LED_PIN_MIN + i, OUTPUT);
    pinMode(BUTTON_PIN_MIN + i, INPUT);
    enableInterrupt(BUTTON_PIN_MIN + i, buttonPressed, RISING);
  }

  /*
   * pseudo number generator init
   */
  randomSeed(analogRead(A1));

  Serial.begin(9600);
  Serial.println("Track to Led Fly Game. Press Key T1 to Start\n");

  /*
   * timer setup: at times up gameOver will be called. We don't start it
   * because the player didn't press T1 yet.
   */
  MiniTimer1.init();
  MiniTimer1.attachInterrupt(gameOver);

}

void loop() {
  // If the player is not playing and gameOver was not called
  if(!isPlaying && !missedLed){
    noInterrupts();
    fadeRedLed();
    interrupts();
  } else if(missedLed){
    // game over set true this global variable: wrong button or times up
    noInterrupts();
    canStart = false;
    interrupts();

    // set at HIGH for 2 seconds the red led
    analogWrite(RED_LED, brightness);
    delay(2 * MILLIS_TO_SECONDS);
    missedLed = false;

    // Now the player can replay the game
    noInterrupts();
    canStart = true;
    interrupts();
  }
  delay(10);
}

/*
 * ISR's button interrupt. It manage all possible ways at button pressing: 
 * - start, at T1 pressing
 * - continue, at the right button
 * - game over, at the wrong one
 */
void buttonPressed(){
  // debouncing
  if(micros() - lastMicros > 200000) {    
    Serial.println(String("pressed") + arduinoInterruptedPin);

    // The game didn't started and T1 is pressed
    if(!isPlaying){
      if(arduinoInterruptedPin == BUTTON_PIN_MIN){
        startGame();
      }
    }
    else if(isPlaying && arduinoInterruptedPin == BUTTON_PIN_MIN + pinOffset){
      // We stop the timer, the button was pressed in time
      MiniTimer1.stop();
      // We turn off the current led and we turn on the next one
      digitalWrite(LED_PIN_MIN + pinOffset, LOW);
      // Score increment
      score++;
      // Next offset calculation
      nextPinOffset();
      Serial.println("OK!");
      digitalWrite(LED_PIN_MIN + pinOffset, HIGH);
      // We reduce tMin in order to reduce the range whom the next expiring time will be calculated
      tMin = tMin * REDUCING_FACTOR;
      // We set the new period of the timer and we start it from the beginning
      MiniTimer1.setPeriod(random(tMin, tMin * K));
      MiniTimer1.reset();
      MiniTimer1.start();
    }
    /*
     * In all the other cases the player pressed a wrong button, then gameOver will be called
     */
    else if(isPlaying) {
      gameOver();
    }
    lastMicros = micros();
  }
}

void fadeRedLed(){
    analogWrite(RED_LED, brightness);
 
    brightness = brightness + verse * FADE_STEP;
    if(brightness == 255 || brightness == 0){
      verse = -(verse);
    }
}

void nextPinOffset(){
  // direction
  int dir = random(0,2);
  int next;
  
  // We move the fly in an adjacent position and then we manage the overflow state
  next = dir ? pinOffset + 1  : pinOffset - 1;
  /* if the next offsed exeeds the maximum range then it's set at minimum (0) */
  if(next > BUTTON_PIN_MAX - BUTTON_PIN_MIN) next = 0;
  /* if the offsed is negative, the maximum must be set */
  if(next < 0) next = BUTTON_PIN_MAX - BUTTON_PIN_MIN;
  
  pinOffset = next;
}

void setGlobalVariables(bool pIsPlaying, int pBrightness, int pVerse, int pPinOffset, int pScore, \
  bool pMissedLed, bool pCanStart, unsigned long pLastMicros, bool pAlreadyOver) {
  score = pScore;
  isPlaying = pIsPlaying;
  brightness = pBrightness;
  verse = pVerse;
  pinOffset = pPinOffset;
  missedLed = pMissedLed;
  canStart = pCanStart;
  lastMicros = pLastMicros;
  alreadyOver = pAlreadyOver;
}
