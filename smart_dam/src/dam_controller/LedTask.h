#ifndef __BLINK_LED_TASK__
#define __BLINK_LED_TASK__

#include "Task.h"
#include "Light.h"

class LedTask : public Task
{
  RiverData *RiverData;
  Light *led;
  enum State
  {
    BL0,
    BL1,
    BL2
  };
  State state;

public:
  LedTask(RiverData *riverData, Led *led);
  void init(int period);
  bool updateTimeAndCheckEvent(int basePeriod);
  void tick();
};

#endif