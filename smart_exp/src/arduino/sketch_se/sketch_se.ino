#include "lib.h"

Scheduler *scheduler;

void setup()
{
  scheduler = new Scheduler();
  scheduler->init(SCHED_PERIOD);

  Experimentation *experimentation = new ExperimentationImpl(Experimentation::State::READY);
  KinematicsData *kinematicsData = new KinematicsDataImpl();

  Sonar *sonar = new SonarImpl(TRIG_PIN, ECHO_PIN);
  sonar->init();

  Led *L1 = new Led(L1_PIN);
  Led *L2 = new Led(L2_PIN);

  Task *stepTask = new StepTask(experimentation, sonar, BSTART_PIN, BSTOP_PIN);
  stepTask->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(stepTask);

  Task *kinematicsTask = new KinematicsTask(experimentation, kinematicsData, sonar, SERVO_MOTOR_PIN);
  scheduler->addTask(kinematicsTask);

  Task *actionTask = new ActionTask(experimentation, PIR_PIN, L1, L2);
  actionTask->init(SCHED_PERIOD);
  scheduler->addTask(actionTask);

  Task *viewerComunicator = new ViewerComunicationTask(experimentation, kinematicsData);
  viewerComunicator->init(SCHED_PERIOD);
  scheduler->addTask(viewerComunicator);

  Task *blinkLedTask = new BlinkLedTask(experimentation, L2);
  blinkLedTask->init(200);
  scheduler->addTask(blinkLedTask);
}

void loop()
{
  scheduler->schedule();
}
