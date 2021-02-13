#ifndef __LIB__
#define __LIB__

#define TRIG_PIN D1
#define ECHO_PIN D0

#define D1_IN_M 1
#define D2_IN_M 0.4

#include "MeasurementImpl.h"
#include "RiverStateImpl.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "ESP8266TimerInterrupt.h"
#include <TimeLib.h>
#include <ArduinoJson.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

extern volatile Measurement* measurement;
extern volatile RiverState* riverState;
extern const double vs;
extern volatile boolean statusLed;
extern volatile uint32_t lastMillis;

extern  String state;
extern volatile float distance;
extern unsigned long timestamp;
extern volatile boolean msgReady;

// Init ESP8266 timer 0
extern ESP8266Timer ITimer;

/* wifi network name */
extern char* ssidName;
/* WPA2 PSK password */
extern char* pwd;
/* service IP address */
extern String address;


void sendData();

void ICACHE_RAM_ATTR readDistanceAndSetState();

void ICACHE_RAM_ATTR blinkLed();

float getDistance();

#endif
