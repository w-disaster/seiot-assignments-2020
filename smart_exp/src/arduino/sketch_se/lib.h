#ifndef __LIB__
#define __LIB__

#define SCHED_PERIOD 50
#define MS_TO_SEC 0.001
#define SEC_TO_MS 1000

#include "Scheduler.h"
#include "ChangeStepTask.h"
#include "ActionTask.h"
#include "BlinkLedTask.h"
#include "KinematicsTask.h"
#include "ExperimentationStepImpl.h"
#include "ViewerComunicationTask.h"
#include "SonarImpl.h"
#include "PirImpl.h"
#include "KinematicsDataImpl.h"

#define BSTART_PIN 9
#define BSTOP_PIN 10
#define PIR_PIN 2
#define TRIG_PIN 6
#define ECHO_PIN 4
#define L1_PIN 12
#define L2_PIN 13
#define SERVO_MOTOR_PIN 7

#endif