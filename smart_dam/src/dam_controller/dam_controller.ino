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
    scheduler->init(10);

    Msg* serialMsg;
    Msg* btMsg;
    
    Task* serialCommTask = new SerialCommTask(btMsg, serialMsg, riverData);
    serialCommTask->init(10);
    scheduler->addTask(serialCommTask);

    /*Task* btCommTask = new BTCommTask(serialMsg, btMsg);
    btCommTask->init(BASE_PERIOD);
    scheduler->addTask(btCommTask);*/

    Task* servoMotorTask = new ServoMotorTask(riverData);
    servoMotorTask->init(10);
    scheduler->addTask(servoMotorTask);
    
    /* initialize tasks */
    Task *ledTask = new LedTask(riverData, led);
    
    /* add tasks to the scheduler */
    scheduler->addTask(ledTask);

    /*//!------------TESTING-----------------
    *///!------------------------------------ 
}

void loop() {
    scheduler->schedule();
}
