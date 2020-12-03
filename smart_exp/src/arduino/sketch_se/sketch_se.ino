#include "Scheduler.h"
#include "StepTask.h"
#include "ActionTask.h"
#include "BlinkLedTask.h"
#include "KinematicsTask.h"
#include "ExperimentationImpl.h"
#include "ViewerComunicationTask.h"
#include "SonarImpl.h"
 #include "PirImpl.h"

#define BSTART_PIN 9
#define BSTOP_PIN 10
#define PIR_PIN 2
#define TRIG_PIN 6
#define ECHO_PIN 4
#define L1_PIN 12
#define L2_PIN 13
#define SERVO_MOTOR_PIN 7
#define SCHED_PERIOD 50

Scheduler *scheduler;

void setup()
{
  Serial.begin(115200);
  scheduler = new Scheduler();
  scheduler->init(SCHED_PERIOD);

  Experimentation *experimentation = new ExperimentationImpl(Experimentation::State::READY);
  
  Sonar* sonar = new SonarImpl();
  sonar->init(TRIG_PIN, ECHO_PIN);

  Led *L1 = new Led(L1_PIN);
  Led *L2 = new Led(L2_PIN);

  Task *stepTask = new StepTask(experimentation, sonar, BSTART_PIN, BSTOP_PIN);
  stepTask->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(stepTask);

  Task *actionTask = new ActionTask(experimentation, PIR_PIN, L1, L2);
  actionTask->init(SCHED_PERIOD);
  scheduler->addTask(actionTask);

  Task* blinkLedTask = new BlinkLedTask(experimentation, L2);
  blinkLedTask->init(200);
  scheduler->addTask(blinkLedTask);

/*  Task *viewerComunicator = new ViewerComunicationTask(experimentation);
  viewerComunicator->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(viewerComunicator);
*/
  Task *kinematicsTask = new KinematicsTask(experimentation, sonar, SERVO_MOTOR_PIN);
  scheduler->addTask(kinematicsTask);
}

void loop()
{
  scheduler->schedule();
}
