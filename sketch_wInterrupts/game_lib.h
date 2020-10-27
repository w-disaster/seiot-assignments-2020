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
#define K 1.5

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
 * makes the game start with variables set.
 */
void startGame();

/*
 * moves the fly left or right randomly.
 */
void nextPinOffset();

/*
 * ends game and shows score.
 */
void gameOver();

void buttonPressed();

void setGlobalVariables(bool pIsPlaying, int pBrightness, int pVerse, int pPinOffset, int pScore, \
  bool pMissedLed, bool pCanStart, unsigned long pLastMicros, bool pAlreadyOver);

#endif
