#ifndef GRADIENT1_H
#define GRADIENT1_H

#include "iLampAnimation.h"
#include "Lamp.h"
#include "FastLED.h"

class Gradient1 : public iLampAnimation
{
	public:
		Gradient1(Lamp* thelamp) : iLampAnimation (thelamp){
		}

		int itterate()
		{
			uint8_t leds = lamp->getNumLeds();
			for(uint8_t i = 0; i < leds; i++) {
				uint8_t h = map(i, 0, leds, s_h, e_h);
				uint8_t v = map(i, 0, leds, s_v, e_v);
				lamp->setLed(i, CHSV(h, 255, v));
			}
			lamp->render();
			return 1000;
		}
	protected:
		uint8_t s_h = 167;
		uint8_t s_v = 43;
		uint8_t e_h = 127;
		uint8_t e_v = 255;
};

#endif // GRADIENT1_H
