#include "smart_exp.h"

Button* buttonStart;
Button* buttonStop;

Scheduler* scheduler;

void setup(){

  buttonStart = new ButtonStart(BUTTON_START_PIN);
  buttonStop = new ButtonStop(BUTTON_STOP_PIN);

  scheduler = new SchedulerImpl();
  Serial.begin(9600);
}

void loop(){
  
}
