#include "BlinkingTask.h"
#include "Arduino.h"

BlinkingTask::BlinkingTask(Light* pLed){ 
  this->pLed = pLed;
}
  
void BlinkingTask::setActive(bool active){
  Task::setActive(active);
  if (active){
    state = ON;
  } else {
    pLed->switchOff();
  }
}
  
void BlinkingTask::tick(){
  switch (state){
    case ON: {
      pLed->switchOn();
      state = OFF;
      break;
    }
    case OFF: {
      pLed->switchOff();
      state = ON;
      break;
    }
  }
}
