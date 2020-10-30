#ifndef _GAME_LIB_H
#define _GAME_LIB_H

#include "MiniTimerOne.h"

/*
 * green led pins
 */
#define LED_PIN_MIN 10
#define LED_PIN_MAX 13

/*
 * button pins
 */
#define BUTTON_PIN_MIN 2
#define BUTTON_PIN_MAX 5

/*
 * red led pin
 */
#define RED_LED 6

/*
 * utilities
 */
#define POT A0
#define FADE_STEP 5
#define REDUCING_FACTOR 7/8
#define K 1.1

/*
 * time conversion 
 */
#define MICROS_TO_SECONDS 1000000
#define MILLIS_TO_SECONDS 1000

/*
 * Extern global variables
 */
extern int pinOffset;
extern int brightness;
extern int verse;
extern int score;
extern double tMin; 
extern bool isPlaying;
extern bool missedLed;
extern bool canStart;
extern bool alreadyOver;
extern volatile unsigned long lastMicros;

/*
 * This function will be called when T1 is pressed
 */
void startGame();

/*
 * This function calculates the next offset of the Led that must turn on and
 * the button that should be pressed
 */
void nextPinOffset();

/*
 * This function is called by MiniTimerOne when a period of time is done 
 * (the player didn't press the button in time) or, when a wrong one is pressed
 */
void gameOver();

/*
 * Led fading function
 */
void buttonPressed();

/*
 * This function is useful to minimize repetitions
 */
void setGlobalVariables(bool pIsPlaying, int pBrightness, int pVerse, int pPinOffset, int pScore, \
  bool pMissedLed, bool pCanStart, unsigned long pLastMicros, bool pAlreadyOver);

#endif
