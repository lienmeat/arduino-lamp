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
//      CRGB col2;
      uint8_t leds = lamp->getNumLeds();
			if(grad > 5) {
				grad = 0;
			}
//      if(blend_step < 255) {
//          for(uint8_t i = 0; i < leds; i++) {
//            uint8_t h = map(i, 0, leds, s_hues[grad], e_hues[grad]);
//            CRGB color = CRGB(255, 255, 255);
//            color.setHue(h);
//            
//            //fract8 blenda = constrain((blend_step/255), 0, 0.99609375); //0.99609375;
//            fract8 blenda = 0.25;
//            col2 = blend(lamp->getLed(i), color, blenda);
//            lamp->setLed(i, color);
//            //lamp->setLed(i, color);
//          }
//          Serial.println(col2.red);
//          blend_step++;
//          Serial.print("blend step: ");
//          Serial.println(blend_step);
//          lamp->render();
//          return 10;
//      }
//			else{
  			for(uint8_t i = 0; i < leds; i++) {
  				uint8_t h = map(i, 0, leds, s_hues[grad], e_hues[grad]);
  				lamp->setLed(i, CHSV(h, 255, 255));
  			}
        lamp->render();
  			grad++;
//        //stay on one gradient for a while
//        //set the blend steps back to default
//        blend_step = 1;
  			return 5000;
//			}
//      return 5000;
		}
	protected:
		uint8_t s_hues[6] = {255, 0, 96, 96, 160, 160};
		uint8_t e_hues[6] = {217, 38, 58, 134, 122, 198};
		uint8_t grad = 0;
    uint8_t blend_step = 1;
};

#endif // GRADIENT2_H
