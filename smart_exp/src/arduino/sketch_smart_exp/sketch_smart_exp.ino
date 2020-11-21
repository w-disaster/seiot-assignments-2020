#include "SchedulerImpl.h"

Scheduler* scheduler;

void setup() {
  scheduler = new SchedulerImpl();
  Serial.begin(115200);
}

void loop() {
  scheduler -> schedule();
  delay(1000);
}
