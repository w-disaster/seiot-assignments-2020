#ifndef __BLINK_LED_TASK__
#define __BLINK_LED_TASK__

#include "Task.h"
#include "Light.h"

class LedTask : public Task
{
  RiverData *riverData;
  Light *led;
  enum State
  {
    L0,
    L1,
    L2
  };
  State state;
  RiverData::DamMode currentMode;

public:
  LedTask(RiverData *riverData, Light *led);
  void init(int period);
  bool updateTimeAndCheckEvent(int basePeriod);
  void tick();
};

#endif