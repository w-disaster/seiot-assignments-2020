#ifndef __LIB__
#define __LIB__

#define TRIG_PIN D0
#define ECHO_PIN D1

#define D1_IN_M 1
#define D2_IN_M 0.4

#include "MeasurementImpl.h"
#include "RiverStateImpl.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

extern Measurement* measurement;
extern RiverState* riverState;
extern const double vs;

extern StaticJsonDocument<100> data;

void send(RiverState::State state, long timestamp, float distance);

void readDistanceAndSend();

float getDistance();

#endif
