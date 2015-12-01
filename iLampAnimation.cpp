#include "iLampAnimation.h"

iLampAnimation::iLampAnimation(Lamp* thelamp)
{
  lamp = thelamp;
  //needed often
  num_leds = lamp->getNumLeds();
  cols = lamp->getCols();
  rows = lamp->getRows();
}

int iLampAnimation::itterate() {
	//noop, override
	return 1;
}

void iLampAnimation::reset() {
	//noop, override
}

void iLampAnimation::interupt(boolean interupted) {
	this->interupted = interupted;
}