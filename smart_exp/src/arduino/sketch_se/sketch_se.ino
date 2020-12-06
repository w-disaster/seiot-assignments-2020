/**
 * Smart Exp by
 * Luca Fabry and Luigi Olivieri.
 */
#include "lib.h"

Scheduler *scheduler;

void setup() {
  scheduler = new Scheduler();
  scheduler->init(SCHED_PERIOD);

  ExperimentationStep *experimentationStep = new ExperimentationStepImpl(ExperimentationStep::Step::READY);
  KinematicsData *kinematicsData = new KinematicsDataImpl();

  Sonar *sonar = new SonarImpl(TRIG_PIN, ECHO_PIN);
  sonar->init();

  Pir* pir = new PirImpl(PIR_PIN);

  Led *L1 = new Led(L1_PIN);
  Led *L2 = new Led(L2_PIN);

  Task *changeStepTask = new ChangeStepTask(experimentationStep, sonar, BSTART_PIN, BSTOP_PIN);
  changeStepTask->init(SLEEP_TIME * MILLIS_TO_SEC);
  scheduler->addTask(changeStepTask);

  Task *kinematicsTask = new KinematicsTask(experimentationStep, kinematicsData, sonar, SERVO_MOTOR_PIN);
  scheduler->addTask(kinematicsTask);

  Task *viewerCommunicator = new ViewerCommunicatorTask(experimentationStep, kinematicsData);
  viewerCommunicator->init(SCHED_PERIOD);
  scheduler->addTask(viewerCommunicator);

  Task *actionTask = new ActionTask(experimentationStep, pir, L1, L2);
  actionTask->init(SCHED_PERIOD);
  scheduler->addTask(actionTask);

  Task *blinkLedTask = new BlinkLedTask(experimentationStep, L2);
  blinkLedTask->init(200);
  scheduler->addTask(blinkLedTask);
}

void loop() {
  scheduler->schedule();
}
