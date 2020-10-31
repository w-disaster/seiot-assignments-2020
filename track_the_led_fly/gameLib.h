#ifndef _GAME_LIB_H
#define _GAME_LIB_H

#include <MiniTimerOne.h>

/*
 * Green led pins
 */
#define LED_PIN_MIN 10
#define LED_PIN_MAX 13

/*
 * Button pins
 */
#define BUTTON_PIN_MIN 2
#define BUTTON_PIN_MAX 5

/*
 * Red led pin
 */
#define RED_LED_PIN 6

/*
 * Utilities
 */
#define POT A0
#define FADE_STEP 5
#define REDUCING_FACTOR 7/8
#define K 1.5
#define MTO_MAX_PERIOD 4194304

/*
 * Time conversion 
 */
#define MICROS_TO_SECONDS 1000000
#define MILLIS_TO_SECONDS 1000

/*
 * Extern global variables
 */
extern long tMin; 
/* Fading variables */
extern int brightness;
extern int verse;
/* Current pin offset */
extern int pinOffset;
/* Current score  */
extern int score;
/* State of the game */
extern bool isPlaying;
/*
 * We set true this variable when the time is up or the player pressed a wrong
 * button: the red led must go HIGH for 2 seconds
 */
extern bool missedLed;
/* Show if the player can start or not */
extern bool canStart;
/* Variable to avoid chain of interrupts */
extern bool alreadyOver;
/* Time in micros for button ISR debounce */
extern volatile unsigned long lastMicros;
/* True if right button is pressed */
extern bool pressed;

/*
 * This function will be called when T1 is pressed
 */
void startGame();

/* 
 *  This function calculates the next pinOffset, the offset from: 
 *  - LED_PIN_MIN => offset + LED_PIN_MIN = current led at HIGH
 *  - BUTTON_PIN_MIN => offset + BUTTON_PIN_MIN = current button to press
 */
void nextPinOffset();

/*
 * This function is called by MiniTimerOne when a period of time is done 
 */
void timesUp();

/*
 * ISR's button interrupt. It manage all possible ways at button pressing: 
 * - start, at T1 pressing;
 * - continue, at the right button pressed;
 * - game over, at the wrong one;
 */
void buttonPressed();

/*
 * Function to set global variables and to minimize repetitions
 */
void setGlobalVariables(bool pIsPlaying, int pBrightness, int pVerse, int pPinOffset, int pScore, \
  bool pMissedLed, bool pCanStart, unsigned long pLastMicros, bool pAlreadyOver, bool pPressed);

#endif
