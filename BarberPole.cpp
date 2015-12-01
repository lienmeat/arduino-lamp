#include "BarberPole.h"
#include "FastLED.h"
#include "lamputil.h"

BarberPole::BarberPole(Lamp* thelamp) : iLampAnimation (thelamp)
{
  lamp = thelamp;
}

int BarberPole::itterate()
{
  if(offset >= 5) {
    offset = 0;
    hue++;
  }
  if(hue > 255) {
    hue = 0;
  }

  uint8_t complement[3];
  hue_triads(hue, complement); //background color is the complement, at half the "value"
  int leds_per = num_leds/2;
  lamp->fill_color(0, leds_per-1, CHSV(complement[1], 255, 85));
  lamp->fill_color(leds_per, num_leds-1, CHSV(complement[2], 255, 85));
  //lamp->fill_color(leds_per*2, num_leds-1, CHSV(complement[3], 255, 85));
  int x = offset;
  for(int y=0; y< 10; y++) {
    if(x >= 5) {
      x = 0;
    }
    lamp->setLed(lamp->XY(x, y), CHSV(complement[0], 255, 255));
    x++;
  }
  offset++;
  lamp->render();
  int base_delay = 10000;
  return base_delay / fps;
}

void BarberPole::reset() {
  cur_led = 0;
  offset = 0;
  hue = 0;
}