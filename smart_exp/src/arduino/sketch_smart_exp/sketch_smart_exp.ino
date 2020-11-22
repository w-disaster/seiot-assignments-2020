#include "SchedulerImpl.h"

Scheduler* scheduler;

void setup() {
  Serial.begin(115200);
  scheduler = new SchedulerImpl();
  scheduler->init(50);
}

void loop() {
  scheduler -> schedule();
}
