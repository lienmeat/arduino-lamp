#include "Lamp.h"

Lamp::Lamp(CRGB *ledarr, int numberofleds, int cols)
{
  //ctor
  setLeds(ledarr);
  setNumLeds(numberofleds);
  setCols(cols);
  setRows(numberofleds / cols);
}

Lamp::~Lamp() {
  //dtor
}

void Lamp::render() {
  if(on()) {
    FastLED.show();
  }
}

int Lamp::on() {
  return is_on;
}

void Lamp::turnOff()
{
  is_on = 0;
  //do other things?
}

void Lamp::turnOn()
{
  is_on = 1;
  //do other things?
}

int Lamp::switchAnimation()
{
  return next_animation;
}

void Lamp::nextAnimation()
{
  next_animation = 0x1;
  //do other things?
}

int Lamp::getRows()
{
  return rows;
}

void Lamp::setRows(int rows)
{
  this->rows = rows;
}

int Lamp::getCols()
{
  return cols;
}

void Lamp::setCols(int cols)
{
  this->cols = cols;
}

int Lamp::getNumLeds()
{
  return num_leds;
}

void Lamp::setNumLeds(int numberofleds)
{
  num_leds = numberofleds;
}

void Lamp::setLed(int led, CRGB led_value) {
  leds[led] = led_value;
}

CRGB Lamp::getLed(int led) {
  return leds[led];
}

CRGB* Lamp::getLeds() {
  return leds;
}

void Lamp::setLeds(CRGB *ledarr)
{
  leds = ledarr;
}

void Lamp::fill_color(int start, int end, CRGB color) {
  for(int i = start; i<=end; i++){
    leds[i] = color;
  }
}

void Lamp::fill_row(int row, CRGB color) {
  int row_end = row*cols-1;
  int row_start = row_end-cols;
  fill_color(row_start, row_end, color);
}

void Lamp::fill_column(int col, CRGB color) {
  for(int i = 0; i < num_leds; i+=cols) {
    int led = i+col-1;
    leds[led] = color;
  }
}

uint16_t Lamp::XY( uint8_t x, uint8_t y)
{
  uint16_t i;  
  i = (y * cols) + x; 
  return i;
}