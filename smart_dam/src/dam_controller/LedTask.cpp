#include "LedTask.h"
#include "Arduino.h"

LedTask::LedTask(RiverData *riverData, Led *led)
{
  this->riverData = RiverData;
  this->led = led;
}

void LedTask::init(int period)
{
  state = BL0;
  Task::init(period);
}

bool LedTask::updateTimeAndCheckEvent(int basePeriod)
{
  State nextState = state;
  bool result = false;
  RiverData::RiverState riverState = this->riverData->getState();
  RiverData::Mode mode = this->riverData->getMode();

  switch (state)
  {
  case BL0: //led sempre spento
    if (riverState == RiverData::RiverState::ALARM_AUTOMATIC)
    {
      nextState = BL1;
      result = true;
      Task::init(getPeriod());
      break;
    }
    break;

  case BL1: // led acceso
    if (mode == RiverData::Mode::AUTOMATIC)
    {
      if (updateAndCheckTime(basePeriod))
      {
        nextState = BL0;
        result = true;
        break;
      }
      break;
    }
    else
    {
      nextState = BL0;
      result = true;
      break;
    }

  case BL2:
    if (state == RiverData::RiverState::ALARM_AUTOMATIC)
    {
      if (updateAndCheckTime(basePeriod))
      {
        nextState = BL1;
        result = true;
        break;
      }
      break;
    }
    else
    {
      nextState = BL0;
      result = true;
      break;
    }
  }
  state = nextState;
  return result;
}

void LedTask::tick()
{
  switch (state)
  {
  case BL0:
  case BL2:
    this->led->switchOff();
    break;

  case BL1:
    this->led->switchOn();
    break;
  }
}