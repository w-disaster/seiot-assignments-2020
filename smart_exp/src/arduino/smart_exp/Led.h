#ifndef __LED_H__
#define __LED_H__

#define BLINKING_STEP 200

class Led
{
public:
  
  /**
   * Turns the led on.
   */
  virtual void turnOn() = 0;

  /**
   * Turns the led off.
   */
  virtual void turnOff()= 0;

  /**
   * Makes the led turn on and off for a period of time specified by 
   * the param seconds.
   */
  virtual void blinkLed(int seconds) = 0;
};
#endif
