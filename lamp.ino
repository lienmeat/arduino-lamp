#include "esp8266server.h"
#include "FastLED.h"
#include "Lamp.h"
#include "animations.h"
//here we have included the animations we want to be able to use

#define LED_DATA_PIN 8
#define NUM_LEDS 50
//#define BRIGHTNESS  64
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

#define MatrixWidth 5
#define MatrixHeight 10

const uint8_t BUTTON_PIN = 2; //purple wire

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
  // setAnimation(99);
  //setup our server
  esp8266ServerSetup(lamp);

  setAnimation(99);
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
  if(anim_at > -1) {
    int anim = url.substring(anim_at+6).toInt();
    setAnimation(anim);
  }
  int color_at = url.indexOf("/color/");
  if(color_at > -1) {
    setAnimation(99);
    //red for now
    lamp->fill_color(0, lamp->getNumLeds()-1, CRGB::Red);
    lamp->render();
  }
  resp+=animation;
  return resp;
}