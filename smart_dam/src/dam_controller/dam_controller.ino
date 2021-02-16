#include "dam_controller.h"

#define LED_PIN 2

/* btService */
MsgServiceBT btService(2, 3);

/* scheduler */
Scheduler scheduler;

/* led */
Light *led;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);


  /* initialize bluetooth service */
  btService.init();

  /* initialize scheduler */
  scheduler.init(BASE_PERIOD);

  RiverData* riverData = new RiverDataImpl(RiverData::NORMAL);

  Task* damServiceCommTask = new DamServiceCommTask(riverData);
  damServiceCommTask->init(200);
  scheduler.addTask(damServiceCommTask);
  

  CommMsg* commMsg = new CommMsg("prova");
  Serial.println(commMsg->getContent());

  /* initialize tasks */
  //Task *blinkingTask = new BlinkingTask(led);

  /* add tasks to the scheduler */
  //schedule.addTask(blinkingTask);
}

void loop() {
  scheduler.schedule();
}
