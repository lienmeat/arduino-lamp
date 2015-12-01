#include "RainbowFill.h"
#include "iLampAnimation.h"
#include "Lamp.h"

RainbowFill::RainbowFill(Lamp* thelamp) : iLampAnimation (thelamp)
{
  //ctor
  //lamp = thelamp;
  hue_step = 255 / lamp->getNumLeds();
}

int RainbowFill::itterate()
{
  for(int i=0; i < num_leds; i++) {
    uint8_t tmp_hue = i * hue_step + hue;
    lamp->setLed(i, CHSV(tmp_hue, 255, 255));
  }
  
  lamp->render();
  hue+=hue_step;
  int base_delay = 10000;
  return base_delay / fps;
}

void RainbowFill::reset() {
  cur_led = -1;
  hue = 0;
  hue_step = 0;
}