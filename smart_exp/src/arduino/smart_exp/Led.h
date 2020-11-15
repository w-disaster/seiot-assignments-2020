#ifndef __LED_H__
#define __LED_H__

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
   * Makes the led turn on and off for the specified period of milliseconds.
   * The led switches between On and Off state every blinkStep milliseconds.
   * After the specified milliseconds have passed the led will turn Off.
   */
  virtual void blinkLed(int milliseconds, int blinkStep) = 0;
};
#endif
