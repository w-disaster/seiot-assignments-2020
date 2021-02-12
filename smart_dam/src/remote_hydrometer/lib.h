#ifndef __LIB__
#define __LIB__

#include "MeasurementImpl.h"
#include "RiverStateImpl.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

extern Measurement* measurement;
extern RiverState* riverState;

void send(RiverState::State state, long timestamp, float distance);

void readDistanceAndSend();

float getDistance();

#endif
