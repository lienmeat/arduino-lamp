#include "FastLED.h"
#include "esp8266server.h"
#include "Lamp.h"
#include "animations.h"
//here we have included the animations we want to be able to use

#define LED_DATA_PIN 8
#define NUM_LEDS 50
//#define BRIGHTNESS  64
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

#define MatrixWidth 4
#define MatrixHeight 10

//const uint8_t BUTTON_PIN = 2; //purple wire

volatile boolean next_animation = false;
volatile boolean on_off = true;

int animation = 0;

CRGB leds[NUM_LEDS];

Lamp* lamp;

iLampAnimation* ta;

void switchAnitmation() {
  if(on_off) {
    next_animation = true;
  }
}

void onOffToggle() {
  //this signifies that the off() function is to take over or end (toggle)
  on_off = !on_off;
  if(on_off) {
    lamp->turnOn();
  }
  else if(on_off == 0) {
    lamp->turnOff();
  }
}


void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  lamp = new Lamp(leds, NUM_LEDS, MatrixWidth);
  //go to white animation
  setAnimation(99);
//  setup our server
  esp8266ServerSetup(lamp);

  setAnimation(0);
}



void loop() {
  // if(next_animation) {
  //   next_animation = false;
  //   animation++;
  //   setAnimation(animation);
  // }
  //Serial.println(animation);
  int animation_delay = ta->itterate();
  int delayed = 0;
  while(delayed <= animation_delay) {
    process_request(&urlRouter);
    delay(1);
    delayed++;
  }
}

void white() {
  lamp->fill_color(0, NUM_LEDS-1, CRGB::White);
  lamp->render();
}

void black() {
  lamp->fill_color(0, NUM_LEDS-1, CRGB::Black);
  lamp->render();
}

void setAnimation(uint8_t whichanimation) {
  Serial.print("Setting animation to: ");
  Serial.println(whichanimation);
  switch(whichanimation) {
    case 0:
      ta = new RainbowFill(lamp);
      break;
    case 1:
      ta = new Fire(lamp);
      break;
    case 2:
      ta = new BarberPole(lamp);
      break;
    // case 3:
    //   ta = new DebugV(lamp);
    //   break;
    case 98:
      //default case re-sets the animation to first, re-runs
      ta = new iLampAnimation(lamp);
      black();
      break;
    case 99:
      //default case re-sets the animation to first, re-runs
      ta = new iLampAnimation(lamp);
      white();
      break;
    case 253:
      ta = new PancakeSort(lamp);
      break;
    case 254:
      ta = new Gradient1(lamp);
      break;
    case 255:
      ta = new Gradient2(lamp);
      break;
    default:
      setAnimation(99);
      break;
  }
  animation = whichanimation;
  next_animation = false;
}

String urlRouter(String url) {
  Serial.println(url);
  String resp = "";
  int anim_at = url.indexOf("/anim/");
  int color_at = url.indexOf("/color/");
  int set_at = url.indexOf("/setindiv/");
  
  if(anim_at > -1) {
    int anim = url.substring(anim_at+6).toInt();
    setAnimation(anim);
  }
  else if(color_at > -1) {
    setAnimation(99);
    CRGB color = parseRGB(url.substring(color_at+7));
    lamp->fill_color(0, lamp->getNumLeds()-1, color);
    lamp->render();
  }
  else if(set_at > -1) {
    setAnimation(99);
    //set all leds according to comma separated values, colors are pipe-separated
    uint8_t led = 0;
    uint8_t num_leds = lamp->getNumLeds();
    int  
    while(led < num_leds) {
      
    }
  }
  //resp+=animation;
  return resp;
}

CRGB parseRGB(String color) {
  uint8_t separators[2] = {};
  
  for(uint8_t i = 0; i < 3; i++) {
    uint8_t offset = 0;
    if(i > 0) {
      offset = separators[i-1];
    }
    separators[i] = color.indexOf("|", offset);
  }
  if(separators[0] && separators[1]) {
    return CRGB(color.substring(0, separators[0]).toInt(), color.substring(separators[0]+1, separators[1]).toInt(), color.substring(separators[1]+1).toInt());
  }
  else{
    //return black
    return CRGB(0, 0, 0);
  }
} 
