#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "Task.h"
#include "Light.h"

class BlinkingTask: public Task {
  
public:
  BlinkingTask(Light* pLed);  
  void setActive(bool active);
  void tick();

private:

  long ts0;	
  Light* pLed;
  enum { INIT, ON, OFF } state;

};

#endif
