#include "dam_controller.h"

#define LED_PIN 2


RiverData* riverData = new RiverData();
/* scheduler */
Scheduler* scheduler = new Scheduler();

/* led */
Light *led;

void setup() {
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);

    /* initialize bluetooth service */
    MsgService.init();
   

    /* initialize scheduler */
    scheduler->init(BASE_PERIOD);

    Msg* serialMsg;
    Msg* btMsg;
    
    Task* serialCommTask = new SerialCommTask(btMsg, serialMsg, riverData);
    serialCommTask->init(BASE_PERIOD);
    scheduler->addTask(serialCommTask);

    Task* servoMotorTask = new ServoMotorTask(riverData);
    servoMotorTask->init(200);
    scheduler->addTask(servoMotorTask);
    
    /* initialize tasks */
    //Task *blinkingTask = new BlinkingTask(led);
    
    /* add tasks to the scheduler */
    //schedule.addTask(blinkingTask);
}

void loop() {
    scheduler->schedule();
}
