#include "FastLED.h"

#define LED_DATA_PIN 8
#define NUM_LEDS 50
//#define BRIGHTNESS  64
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

#define LED_ROWS 10

#define LED_COLS 5

const int BUTTON_PIN = 2;

volatile int next_animation = 0;
volatile int on_off = 1;

//speed animations should run at
//larger is SLOWER (should never be 0 though!)
int ANIMATION_SPEED = 1;

int animation = 0;

CRGB leds[NUM_LEDS];

//delay fn which will stop delaying if
//quitAnimation() is true
void aDelay(int millis) {  
  while(!quitAnimation() && millis > 0) {
    delay(1);
    millis--;
  }
}

void switchAnitmation() {
  if(on_off) {
    digitalWrite(9, HIGH);
    next_animation = 1;
  }
}

void onOffToggle() {
  digitalWrite(9, LOW);
  //this signifies that the off() function is to take over or end (toggle)
  on_off = !on_off;  
}

/**
 * Simple way to know to kill the current animation
 * @return int
 */
bool quitAnimation() {
  return (!on_off || next_animation);
}


void setup() {
  Serial.begin(9600);  
  pinMode(9, OUTPUT);
  
  digitalWrite(9, LOW);
  
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  //adds hardware interupt to digital pin 2 (interupt 0)
  attachInterrupt(0, switchAnitmation, RISING);
  //adds hardware interupt to digital pin 3 (interupt 1)
  attachInterrupt(1, onOffToggle, RISING);
}

void loop() {
  if(on_off == 0) {
    off();    
    //want to make certain we
    //go to the white animation
    //and stay there until a button is pressed
    animation = 0;
    next_animation = 0;
  }
  else if(next_animation) {    
    digitalWrite(9, LOW);
    delay(100);
    digitalWrite(9, HIGH);
    delay(100);
    digitalWrite(9, LOW);
    delay(100);
    digitalWrite(9, HIGH);    
    delay(100);
    digitalWrite(9, LOW);
    Serial.println("here");
    next_animation = 0;
    animation++;
  }

  run_animation();   
}

void run_animation() {

  switch(animation) {
    case 0:      
      white(); //white is always the first animation!
      break;    
    case 1:
      colorWheel();
      break;
    case 2:
      cop_light();      
      break;
    case 3:
      rainbow();
      break;
    case 4:
      barber_pole();
      break;
    default:      
      //default case re-sets the animation to first, re-runs
      animation = 0;
      //this happens so soon after the button press
      //that it's necessary to software de-bounce the button here! (esentially)
      //@todo: look into if there is a way to prevent this easily.
      next_animation = 0;
      run_animation();
      break;
  }
}


void white() {
  fill_color(leds, 0, NUM_LEDS-1, CRGB::White);
  FastLED.show();
  while(!quitAnimation()) {
    aDelay(500);
    //noop, just hang out until next animation...
  }
}

void off() {  
  turnoff_animation();
  while(!on_off) {
    aDelay(500);
  }
  //make cool bootup animation play here
  turnon_animation();  
}

/**
 * Quick animation that plays once through when turning on 
 */
void turnon_animation() {
  for(int i = 0; i<=250; i+=10) {
    uint8_t brightness = brighten8_video(i);
    fill_color(leds, 0, NUM_LEDS-1, CRGB(brightness, brightness, brightness));    
    delay(10);
    FastLED.show();    
  }  
}

/**
 * Quick animation that plays once through when turning off 
 */
void turnoff_animation() {
  for(int i = 250; i>=0; i-=10) {
    uint8_t brightness = dim8_video(i);
    fill_color(leds, 0, NUM_LEDS-1, CRGB(brightness, brightness, brightness));    
    delay(10);
    FastLED.show();
  }
}

void rainbow() {
  CRGB colors[6] = {CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Purple};
  while(!quitAnimation()) {
    for(int c = 0; c<6; c++) {
      for(int i = 0; i<NUM_LEDS; i++) {
        if(quitAnimation()) {
          return;
        }
        leds[i] = colors[c];        
      }
      
      FastLED.show();
      aDelay(2000 * ANIMATION_SPEED);
    }
  }
}

/**
 * HSV color wheel hues 
 */
void colorWheel() {
  //color with max brightness, max saturation, 0 hue to start
  CRGB color = CHSV(0, 255, 255);
  for(int i = 0; i<256; i++) {
    if(quitAnimation()) {
      return;
    }
    color.setHue(i);
    fill_color(leds, 0, NUM_LEDS-1, color);
    FastLED.show();
    aDelay(100 * ANIMATION_SPEED);
    if(i%32 == 0) {
      //stop for 10 seconds * ANIMATION_SPEED every 32 hues (r o y g b c v p)
      aDelay(10000 * ANIMATION_SPEED);
    }
  }
}

void cop_light() {
  CRGB color1 = CRGB::Red;
  CRGB color2 = CRGB::Blue;
  while(!quitAnimation()) {
    for(int i = 1; i<=LED_COLS; i++) {
      if(quitAnimation()) {
        break;
      }
      fill_color(leds, 0, NUM_LEDS-1, CRGB::Black);
      fill_column(i, color1);
      FastLED.show();
      aDelay(1000 * ANIMATION_SPEED);      
    }    
    for(int i = 1; i<=LED_COLS; i++) {
      if(quitAnimation()) {
        break;
      }
      fill_color(leds, 0, NUM_LEDS-1, CRGB::Black);
      fill_column(i, color2);
      FastLED.show();
      aDelay(1000 * ANIMATION_SPEED);      
    }
  }
}

void barber_pole() {
  //where the animation starts
  int offset = 0;
  CRGB red = CRGB::Red;
  CRGB white = CRGB::White;
  while(!quitAnimation()) {
    //fill with white
    fill_color(leds, 0, NUM_LEDS-1, white);
    if(offset > 5) {
      offset = 0;
    }
    //red on every 6th led (1 per row)
    for(int i=offset; i<NUM_LEDS; i+=6) {
      Serial.println(i);
      leds[i] = red;
    }
    offset++;
    FastLED.show();
    aDelay(3000 * ANIMATION_SPEED);
  }
}


void fill_color(struct CRGB *leds, int start, int end, const struct CRGB& color) {
  for(int i = start; i<=end; i++){
    leds[i] = color;
  }
}

void fill_row(int row, CRGB color) {
  int row_end = row*LED_COLS-1;
  int row_start = row_end-LED_COLS;
  fill_color(leds, row_start, row_end, color);
}

void fill_column(int col, CRGB color) {
  for(int i = 0; i < NUM_LEDS; i+=LED_COLS) {
    int led = i+col-1;
    Serial.println(led);
    leds[led] = color;    
  }
}