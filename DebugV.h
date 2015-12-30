#ifndef DEBUGV_H
#define DEBUGV_H

#include "iLampAnimation.h"
#include "Lamp.h"
 #include "FastLED.h"

class DebugV : public iLampAnimation
{
	public:
		DebugV(Lamp* thelamp) : iLampAnimation (thelamp){}

		int itterate()
		{
			uint8_t cols = lamp->getCols();
			uint8_t leds = lamp->getNumLeds();
			for(uint8_t i=0; i < leds; i++) {
				uint8_t m = i%cols;
				uint8_t hue = 127 / m;
				lamp->setLed(i, CHSV(hue, 255, 255));
			}
			lamp->render();
			return 1;
		}
};

#endif // DEBUGV_H
