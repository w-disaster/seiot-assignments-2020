#include "dam_controller.h"

#define LED_PIN 2

/* btService */
MsgServiceBT* btService;

/* scheduler */
Scheduler* scheduler;

/* led */
Light *led;

void setup() {
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    
    btService = new MsgServiceBT(2, 3);
    /* initialize bluetooth service */
    btService->init();
    MsgService.init();
    
    /* initialize scheduler */
    scheduler->init(BASE_PERIOD);
    
    RiverData* riverData = new RiverData();
    
    Task* serialCommTask = new SerialCommTask(riverData, btService);
    serialCommTask->init(200);
    scheduler->addTask(serialCommTask);
    
    /* initialize tasks */
    //Task *blinkingTask = new BlinkingTask(led);
    
    /* add tasks to the scheduler */
    //schedule.addTask(blinkingTask);
}

void loop() {
    scheduler->schedule();
}
