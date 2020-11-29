#include "Scheduler.h"
#include "StepTask.h"
#include "KinematicsTask.h"
#include "ExperimentationImpl.h"
#include "ViewerComunicationTask.h"

#define BSTART_PIN 3
#define BSTOP_PIN 8
#define PIR_PIN 2
#define TRIG_PIN 6
#define ECHO_PIN 11

Scheduler *scheduler;

void setup()
{
  Serial.begin(115200);
  scheduler = new Scheduler();
  scheduler->init(20);

  Experimentation *experimentation = new ExperimentationImpl(Experimentation::State::READY);

  Task *stepTask = new StepTask(experimentation, BSTART_PIN, BSTOP_PIN, PIR_PIN);
  stepTask->init(SLEEP_TIME * MILLIS_TO_SEC);

  ViewerComunication *viewerComunicator = new ViewerComunicationTask(experimentation);
  viewerComunicator->init(SLEEP_TIME * MILLIS_TO_SEC);

  Task *kinematicsTask = new KinematicsTask(experimentation, TRIG_PIN, ECHO_PIN);

  scheduler->addTask(stepTask);
  scheduler->addTask(kinematicsTask);
  scheduler->addTask(viewerComunicator);
}

void loop()
{
  scheduler->schedule();
}
