#include "lib.h"

Scheduler *scheduler;

void setup()
{
  Serial.begin(115200);
  scheduler = new Scheduler();
  scheduler->init(SCHED_PERIOD);

  Experimentation *experimentation = new ExperimentationImpl(Experimentation::State::READY);
  
  Sonar* sonar = new SonarImpl(TRIG_PIN, ECHO_PIN);
  sonar->init();

  Led *L1 = new Led(L1_PIN);
  Led *L2 = new Led(L2_PIN);

  Task *stepTask = new StepTask(experimentation, sonar, BSTART_PIN, BSTOP_PIN);
  stepTask->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(stepTask);
/*
  Task *viewerComunicator = new ViewerComunicationTask(experimentation);
  viewerComunicator->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(viewerComunicator);
*/
  Task *actionTask = new ActionTask(experimentation, PIR_PIN, L1, L2);
  actionTask->init(SCHED_PERIOD);
  scheduler->addTask(actionTask);

  Task* blinkLedTask = new BlinkLedTask(experimentation, L2);
  blinkLedTask->init(200);
  scheduler->addTask(blinkLedTask);

  Task *kinematicsTask = new KinematicsTask(experimentation, sonar, SERVO_MOTOR_PIN);
  scheduler->addTask(kinematicsTask);
}

void loop()
{
  scheduler->schedule();
}
