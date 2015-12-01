#ifndef BARBERPOLE_H
#define BARBERPOLE_H

#include "iLampAnimation.h"
#include "Lamp.h"

class BarberPole : public iLampAnimation
{
  public:
    BarberPole(Lamp* lamp);
    int itterate();
    void reset();
  protected:
    int cur_led = 0;
    int offset = 0;
    uint8_t hue = 0;
    uint8_t fps = 60;
  private:
};

#endif // BarberPole_H
