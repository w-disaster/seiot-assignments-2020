#include "Scheduler.h"
#include "ExperimentationStepTask.h"
#include "KinematicsTask.h"
#include "ExperimentationImpl.h"

#define BSTART_PIN 3
#define BSTOP_PIN 8
#define PIR_PIN 2
#define TRIG_PIN 6
#define ECHO_PIN 11

Scheduler* scheduler;

void setup() {
  Serial.begin(115200);
  scheduler = new Scheduler();
  scheduler->init(20);

  Experimentation* experimentation = new ExperimentationImpl(Experimentation::State::READY);

  Task* expStepTask = new ExperimentationStepTask(experimentation, BSTART_PIN, BSTOP_PIN, PIR_PIN);
  expStepTask->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(expStepTask);

  Task* kinematicsTask = new KinematicsTask(experimentation, TRIG_PIN, ECHO_PIN);
  scheduler->addTask(kinematicsTask);
}

void loop() {
  scheduler->schedule();
}
