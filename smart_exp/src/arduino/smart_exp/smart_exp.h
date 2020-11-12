/**
 * File to better organize included libraries and defines for main Arduino file.
 */
#ifndef __SMART_EXP_H___
#define __SMART_EXP_H___

/**
 * libraries.
 */
#include "ButtonStart.h"
#include "ButtonStop.h"

/**
 * pins.
 */
#define BUTTON_START_PIN 2
#define BUTTON_STOP_PIN 3

/**
 * utility.
 */
#define SLEEP_TIME 5 
#define MAX_TIME 20 
#define ERROR_TIME 2 
#define MIN_FREQ 1 
#define MAX_FREQ 50 
//#define MAXVEL tbd

#endif
