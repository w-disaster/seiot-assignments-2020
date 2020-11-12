#ifndef __BUTTON_H__
#define __BUTTON_H__

class Button
{

public:
  /** 
   * Starts the apropriate routine for the button pressed.
  */
  virtual void pressButton() = 0;
};

#endif
