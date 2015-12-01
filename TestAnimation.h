#ifndef TESTANIMATION_H
#define TESTANIMATION_H

#include "iLampAnimation.h"
#include "Lamp.h"

class TestAnimation : public iLampAnimation
{
  public:
    TestAnimation(Lamp* lamp);
    int itterate();
    void reset();
  protected:
    int cur_led = 0;
  private:
};

#endif // TESTANIMATION_H
