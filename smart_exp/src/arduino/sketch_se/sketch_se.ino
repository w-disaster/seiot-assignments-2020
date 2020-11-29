#include "Scheduler.h"
#include "StepTask.h"
#include "ActionTask.h"
#include "KinematicsTask.h"
#include "ExperimentationImpl.h"
#include "ViewerComunicationTask.h"
#include "PirImpl.h"

#define BSTART_PIN 3
#define BSTOP_PIN 8
#define PIR_PIN 2
#define TRIG_PIN 6
#define ECHO_PIN 11
#define L1_PIN 12
#define L2_PIN 13
#define SCHED_PERIOD 20

Scheduler *scheduler;

void setup()
{
  Serial.begin(115200);
  scheduler = new Scheduler();
  scheduler->init(SCHED_PERIOD);

  Experimentation *experimentation = new ExperimentationImpl(Experimentation::State::READY);

  Pir *pir = new PirImpl(PIR_PIN);
  Led *L1 = new Led(L1_PIN);
  Led *L2 = new Led(L2_PIN);

  Task *stepTask = new StepTask(experimentation, pir, BSTART_PIN, BSTOP_PIN);
  stepTask->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(stepTask);

  Task *actionTask = new ActionTask(experimentation, L1, L2, pir);
  actionTask->init(SCHED_PERIOD);
  scheduler->addTask(actionTask);

/*  Task *viewerComunicator = new ViewerComunicationTask(experimentation);
  viewerComunicator->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(viewerComunicator);
*/
  Task *kinematicsTask = new KinematicsTask(experimentation, TRIG_PIN, ECHO_PIN);
  scheduler->addTask(kinematicsTask);
}

void loop()
{
  scheduler->schedule();
}
