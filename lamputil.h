#include "FastLED.h"

uint8_t* hue_complement(uint8_t hue, uint8_t* hues);

uint8_t* hue_split_complements(uint8_t hue, uint8_t* hues);

uint8_t* hue_triads(uint8_t hue, uint8_t* hues);

uint8_t* hue_tetrads(uint8_t hue, uint8_t* hues);

uint8_t* hue_analogous(uint8_t hue, uint8_t* hues);