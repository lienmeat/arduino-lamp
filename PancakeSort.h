#ifndef PANCAKESORT_H
#define PANCAKESORT_H

#include "iLampAnimation.h"
#include "Lamp.h"
#include "FastLED.h"

class PancakeSort : public iLampAnimation
{
	public:
		PancakeSort(Lamp* thelamp) : iLampAnimation (thelamp){
			num_pancakes = lamp->getRows();
			createPancakes();
		}

		int itterate()
		{
			if(sort_pos >= num_pancakes) {
				reset();
				render();
				return 3000;
			}
			if(action > 2) {
				action = 0;
			}
			switch(action) {
				case 0:
					findBiggest(sort_pos);
					break;
				case 1:
					flip(biggest_index);
					biggest_index = num_pancakes -1;
					break;
				case 2:
					flip(sort_pos);
					biggest_index = sort_pos;
					sort_pos++;
					break;
			}
			action++;
			render();
			return 3000;
		}

		void reset() {
			createPancakes();
			sort_pos = 0;
			action = 0;
		}
	protected:
		uint8_t num_pancakes;
		uint8_t pancakes[100];
		//position to start sorting from
		uint8_t sort_pos = 0;
		//search: 0, fliptop: 1, flipdone: 1
		uint8_t action = 0;
		
		uint8_t biggest_index = 0;

		void createPancakes() {
			uint8_t hue_step = 224 / num_pancakes;
			for(uint8_t i=0; i < num_pancakes; i++) {
				pancakes[i] = hue_step * i;
			}
			randomSeed(analogRead(A0));
			//crappy naive shuffle
			for(uint8_t i=0; i < num_pancakes; i++) {
				uint8_t r = random(0, i);
				swap(i, r);
			}
		}
		void print() {
			Serial.println("Pancakes: ");
			for(uint8_t i=0; i < num_pancakes; i++) {
				Serial.println(pancakes[i]);
			}
			Serial.println(" ");
			Serial.println(" ");
		}
		void swap(uint8_t a, uint8_t b) {
			uint8_t tmp = pancakes[a];
			pancakes[a] = pancakes[b];
			pancakes[b] = tmp;
		}

		void flip(uint8_t bottom) {
			uint8_t count = 1;
			uint8_t i = bottom;
			while(i < num_pancakes-count) {
				swap(i, (num_pancakes - count));
				count++;
				i++;
			}
		}

		uint8_t findBiggest(uint8_t offset) {
			biggest_index = offset;
			for(uint8_t i = offset; i < num_pancakes; i++) {
				if(pancakes[i] > pancakes[biggest_index]) {
					biggest_index = i;
				}
			}
			return biggest_index;
		}
		void render() {
			for(uint8_t i = 0; i < num_pancakes; i++) {
				if(i < sort_pos || i == biggest_index) {
					renderPancake(i, 255);
				}else {
					renderPancake(i, 127);
				}
			}
			lamp->setLed(49, CRGB::Black);
			lamp->setLed(48, CRGB::Black);
			Serial.println("Rendered");
			lamp->render();
		}

		void renderPancake(uint8_t pancake, uint8_t brightness) {
			uint8_t cols = lamp->getCols();
			// uint8_t start = ;
			lamp->fill_color(pancake * cols, pancake * cols + cols - 1, CHSV(pancakes[pancake], 255, brightness));
			// for(uint8_t i = start; i < cols; i++) {
			// 	lamp->setLed(i, );
			// }
		}
};

#endif // PANCAKESORT_H
