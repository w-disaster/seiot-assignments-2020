#include "LedTask.h"
#include "Arduino.h"

LedTask::LedTask(RiverData *riverData, Light *led)
{
  this->riverData = riverData;
  this->led = led;
}

void LedTask::init(int period)
{
  state = L0;
  currentMode = riverData->getDamMode();
  Task::init(period);
}

bool LedTask::updateTimeAndCheckEvent(int basePeriod)
{
  State nextState = state;
  bool result = false;
  RiverData::RiverState riverState = this->riverData->getRiverState();
  RiverData::DamMode mode = this->riverData->getDamMode();

  switch (state)
  {
  case L0: //led rimane costante a meno che non cambi lo stato
    if (currentMode != mode)
    {
      if (riverState == RiverData::RiverState::ALARM)
      {
        // è in allarme si accende
        nextState = L1;
      }
      else
      {
        // non è in allarme si spegne
        nextState = L2;
      }
      currentMode = mode;
      result = true;
      Task::init(getPeriod());
    }
    break;

  case L1: // led acceso
    if (mode == RiverData::DamMode::AUTO)
    {
      // blink
      if (updateAndCheckTime(basePeriod))
      {
        nextState = L2;
        result = true;
        break;
      }
      break;
    }
    else
    {
      // sempre acceso
      nextState = L0;
      result = true;
      break;
    }

  case L2:
    if (riverState == RiverData::RiverState::ALARM)
    {
      // blink
      if (updateAndCheckTime(basePeriod))
      {
        nextState = L1;
        result = true;
        break;
      }
      break;
    }
    else
    {
      // sempre spento
      nextState = L0;
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
  case L0:
  case L2:
    this->led->switchOff();
    break;

  case L1:
    this->led->switchOn();
    break;
  }
}