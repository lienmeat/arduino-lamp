#include "Fire.h"
#include "FastLED.h"

Fire::Fire(Lamp* thelamp) : iLampAnimation(thelamp) {}

int Fire::itterate()
{
	random16_add_entropy( random());
	int num_leds = lamp->getNumLeds();
	// Array of temperature readings at each simulation cell
	byte heat[num_leds];

	// Step 1.  Cool down every cell a little
	for( int i = 0; i < num_leds; i++) {
		heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / num_leds) + 2));
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for( int k= num_leds - 1; k >= 2; k--) {
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if( random8() < sparking ) {
		int y = random8(7);
		heat[y] = qadd8( heat[y], random8(160,255) );
	}

	// Step 4.  Map from heat cells to LED colors
	for( int j = 0; j < num_leds; j++) {
		lamp->setLed(j, HeatColor(heat[j]));
	}
	lamp->render();
	int base_delay = 10000;
	return base_delay / fps;
}

void Fire::reset()
{
	fps = 60;
	cooling = 55;
	sparking = 120;
}