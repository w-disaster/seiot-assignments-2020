#ifndef __RH__
#define __RH__

#include "Led.h"
#include "SonarClass.h"
#include "ControllerTask.h"
#include "BlinkLedTask.h"
#include "SendTask.h"
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define BAUD_RATE 9600
#define JSON_DIM 100

extern Ticker controllerTaskTicker;
extern Ticker sendTaskTicker;
extern Ticker blinkLedTaskTicker;

extern boolean msgReady;
extern float distance;

extern double d1_in_m;
extern double d2_in_m;

/* NTP client to fetch timestamp */
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

/* Wi-Fi config */
extern char* ssidName;
extern char* pwd;
extern String address;

/* Builds the json and sends the data */
extern int sendData();

#endif
