#include "iLampAnimation.h"
#include "Lamp.h"

class Fire : public iLampAnimation
{
	public:
		Fire(Lamp* lamp);
		int itterate();
		void reset();
	protected:
    byte heat[100];
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
