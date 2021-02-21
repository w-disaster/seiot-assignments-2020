#ifndef __LIB__
#define __LIB__

#define TRIG_PIN D1
#define ECHO_PIN D0

#define D1_IN_M 1.0
#define D2_IN_M 0.4
#define LED_PIN D2

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>  //Ticker Library
//#include <TimeLib.h>
#include <ArduinoJson.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

extern double d1_in_m;
extern double d2_in_m;

extern const double vs;
extern volatile boolean statusLed;
extern volatile uint32_t lastMillis;

extern float distance;
extern unsigned long timestamp;
extern boolean msgReady;
extern boolean isStateChanged; 
extern boolean mustDetachLedISR;

enum State{
  IDLE,
  NORMAL,
  PREALARM,
  ALARM
};

extern State state;
extern State precState;

extern Ticker blinker;

/* NTP client to fetch timestamp */
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

/* wifi network name */
extern char* ssidName;
/* WPA2 PSK password */
extern char* pwd;
/* service IP address */
extern String address;

void sendData();

void readDistanceAndSetState();

void setMsgReady();

void blinkLed();

float getDistance();

#endif
