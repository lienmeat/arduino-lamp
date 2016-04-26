#include "iLampAnimation.h"
#include "Lamp.h"

class LMFAO : public iLampAnimation
{
  public:
    LMFAO(Lamp* lamp);
    int itterate();
    void reset();
    CRGB LMFAOColor(uint8_t range);
  protected:
    byte lmfao[100];
    uint8_t fps = 60;
    // COOLING: How much does the air cool as it rises?
    // Less cooling = taller flames.  More cooling = shorter flames.
    // Default 50, suggested range 20-100 
    uint8_t cooling = 55;
    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    uint8_t sparking = 50;
};

LMFAO::LMFAO(Lamp* thelamp) : iLampAnimation(thelamp) {
  int num_leds = lamp->getNumLeds();
  // Array of temperature readings at each simulation cell
  byte lmfao[num_leds];
}

int LMFAO::itterate()
{
  random16_add_entropy( random());
  int num_leds = lamp->getNumLeds();

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < num_leds; i++) {
    lmfao[i] = qsub8( lmfao[i],  random8(0, ((cooling * 10) / num_leds) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= num_leds - 1; k >= 2; k--) {
    lmfao[k] = (lmfao[k - 1] + lmfao[k - 2] + lmfao[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < sparking ) {
    int y = random8(7);
    lmfao[y] = qadd8( lmfao[y], random8(160,255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < num_leds; j++) {
    lamp->setLed(j, LMFAOColor(lmfao[j]));
  }
  lamp->render();
  int base_delay = 1000;
  return base_delay / fps;
}

void LMFAO::reset()
{
  fps = 60;
  cooling = 55;
  sparking = 120;
}

//CRGB LMFAO::LMFAOColor(uint8_t range) {
//  CRGB colors[4] = {CRGB(0, 0, 0), CRGB(255, 0, 170), CRGB(0, 255, 255), CRGB(255, 255, 0)};
//  uint8_t pick = map(range, 0, 255, 0, 3);
//  return colors[pick];
//}

CRGB LMFAO::LMFAOColor( uint8_t temperature)
{
    CRGB heatcolor;

    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 192);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        // we're in the hottest third (
        heatcolor.r = heatramp; // full red
        heatcolor.g = heatramp; // ramp up green
        heatcolor.b = 0; // no blue

    } else if( t192 & 0x40 ) {
        // we're in the middle third
        
        heatcolor.r = heatramp; // full red
        heatcolor.g = 0; // no green
        heatcolor.b = map(heatramp, 0, 255, 0, 170); // ramp up blue

    } else {
        // we're in the coolest third
        heatcolor.r = 0; // ramp up red
        heatcolor.g = heatramp; // no green
        heatcolor.b = heatramp; // no blue
    }

    return heatcolor;
}
