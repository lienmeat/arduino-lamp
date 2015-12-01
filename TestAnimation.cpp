#include "TestAnimation.h"
#include "FastLED.h"

TestAnimation::TestAnimation(Lamp* thelamp) : iLampAnimation (thelamp)
{

}

int TestAnimation::itterate()
{
  int num_leds = lamp->getNumLeds();
  if(cur_led >= num_leds) {
    reset();
  }
  lamp->setLed(cur_led, CRGB::Red);
  lamp->render();
  cur_led++;
  return 1;
}

void TestAnimation::reset()
{
  cur_led = 0;
}