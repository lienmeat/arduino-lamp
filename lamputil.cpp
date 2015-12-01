#include "lamputil.h"
#include "FastLED.h"

uint8_t* hue_complement(uint8_t hue, uint8_t* hues) {
	hues[0] = hue;
	hues[1] = hue + 127;
	return hues;
}

uint8_t* hue_split_complements(uint8_t hue, uint8_t* hues) {
	hues[0] = hue;
	hues[1] = hue + 110;
	hues[2] = hue + 144;
	return hues;
}

uint8_t* hue_triads(uint8_t hue, uint8_t* hues) {
	hues[0] = hue;
	hues[1] = hue + 85;
	hues[2] = hue + 170;
	return hues;
}

uint8_t* hue_tetrads(uint8_t hue, uint8_t* hues) {
	hues[0] = hue;
	hues[1] = hue + 64;
	hues[2] = hue + 127;
	hues[3] = hue + 191;
	return hues;
}

uint8_t* hue_analogous(uint8_t hue, uint8_t* hues) {
	hues[0] = hue;
	hues[1] = hue + 21;
	hues[2] = hue + 234;
	return hues;
}