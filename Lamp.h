#ifndef LAMP_H
#define LAMP_H

#include "FastLED.h"

class Lamp
{
  public:
    Lamp(CRGB *ledarr, int numberofleds, int cols);
    ~Lamp();
    void render();
    int on();
    void turnOff();
    void turnOn();
    int switchAnimation();
    void nextAnimation();
    int getRows();
    void setRows(int rows);
    int getCols();
    void setCols(int cols);
    int getNumLeds();
    void setNumLeds(int numberofleds);
    void setLed(int led, CRGB led_value);
    CRGB getLed(int led);
    CRGB* getLeds();
    void setLeds(CRGB (* ledarr));
    //these probably don't belong here, but whatever
    void fill_color(int start, int end, CRGB color);
    void fill_row(int row, CRGB color);
    void fill_column(int col, CRGB color);
    uint16_t XY( uint8_t x, uint8_t y);
  protected:
    CRGB* leds;
    int is_on = 1;
    int next_animation = 0;
    int num_leds = 50;
    int cols = 5;
    int rows = 10;
  private:
};

#endif // LAMP_H
