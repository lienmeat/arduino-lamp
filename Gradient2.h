#ifndef GRADIENT2_H
#define GRADIENT2_H

#include "iLampAnimation.h"
#include "Lamp.h"
#include "FastLED.h"

class Gradient2 : public iLampAnimation
{
	public:
		Gradient2(Lamp* thelamp) : iLampAnimation (thelamp){
		}

		int itterate()
		{
			if(grad > 5) {
				grad = 0;
			}
			uint8_t leds = lamp->getNumLeds();
			for(uint8_t i = 0; i < leds; i++) {
				uint8_t h = map(i, 0, leds, s_hues[grad], e_hues[grad]);
				lamp->setLed(i, CHSV(h, 255, 255));
			}
			grad++;
			lamp->render();
			return 5000;
		}
	protected:
		uint8_t s_hues[6] = {255, 0, 96, 96, 160, 160};
		uint8_t e_hues[6] = {217, 38, 58, 134, 122, 198};
		uint8_t grad = 0;
};

#endif // GRADIENT2_H
