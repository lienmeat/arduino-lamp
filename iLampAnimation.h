#ifndef ILAMPANIMATION_H
#define ILAMPANIMATION_H

#include "Lamp.h"

class iLampAnimation
{
public:
    iLampAnimation(Lamp* thelamp);
    virtual int itterate();
    virtual void reset();
    void interupt(boolean interupted);
protected:
    Lamp* lamp;
    uint8_t num_leds;
    uint8_t rows;
    uint8_t cols;
    uint8_t fps = 60;
    boolean interupted = false;
private:
};

#endif // ILAMPANIMATION_H
