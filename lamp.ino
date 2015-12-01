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

const uint8_t BUTTON_PIN = 2; //purple

volatile boolean next_animation = false;
volatile boolean on_off = true;

//speed animations should run at
//larger is SLOWER (should never be 0 though!)
// int ANIMATION_SPEED = 1;

int animation = 0;

// boolean setup_esp8266 = false;

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

/**
 * Simple way to know to kill the current animation
 * @return int
 */
// boolean quitAnimation() {
//   return (!on_off || next_animation);
// }


void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  lamp = new Lamp(leds, NUM_LEDS, MatrixWidth);
  //adds hardware interupt to digital pin 2 (interupt 0)
  // attachInterrupt(0, switchAnitmation, FALLING);
 
  //go to white animation
  setAnimation(99);
  //setup our server
  esp8266ServerSetup(lamp);

  //adds hardware interupt to digital pin 3 (interupt 1)
  // attachInterrupt(1, serialRecieved, RISING);
}



void loop() {
  // if(on_off == 0) {
  //   off();    
  //   //want to make certain we
  //   //go to the white animation
  //   //and stay there until a button is pressed
  //   next_animation = 0;
  // }
  if(next_animation) {
    // lamp->fill_color(0, NUM_LEDS-1, CHSV(HUE_AQUA, 255, 85));
    // lamp->render();
    // //delay(100);
    // lamp->fill_color(0, NUM_LEDS-1, CHSV(HUE_AQUA, 255, 0));
    // lamp->render();
    // //delay(100);
    // lamp->fill_color(0, NUM_LEDS-1, CHSV(HUE_AQUA, 255, 85));
    // lamp->render();
    // //delay(100);
    // lamp->fill_color(0, NUM_LEDS-1, CHSV(HUE_AQUA, 255, 0));
    // lamp->render();
    //delay(100);
    next_animation = false;
    animation++;
    setAnimation(animation);
  }
  Serial.print("ra: ");

  Serial.println(animation);
  int animation_delay = ta->itterate();
  // Serial.println(animation_delay);
  // int animation_delay = 10;
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

// void off() {  
//   turnoff_animation();
//   while(!on_off) {
//     delay(500);
//   }
//   //make cool bootup animation play here
//   turnon_animation();  
// }

/**
 * Quick animation that plays once through when turning on 
 */
// void turnon_animation() {
//   for(uint8_t i = 0; i<=250; i+=10) {
//     uint8_t brightness = brighten8_video(i);
//     lamp->fill_color(0, NUM_LEDS-1, CRGB(brightness, brightness, brightness));    
//     delay(10);
//     lamp->render();
//   }  
// }

// /**
//  * Quick animation that plays once through when turning off 
//  */
// void turnoff_animation() {
//   for(uint8_t i = 250; i>=0; i-=10) {
//     uint8_t brightness = dim8_video(i);
//     lamp->fill_color(0, NUM_LEDS-1, CRGB(brightness, brightness, brightness));    
//     delay(10);
//     lamp->render();
//   }
// }

void setAnimation(uint8_t whichanimation) {
  switch(whichanimation) {
    case 0:
      ta = new BarberPole(lamp);
      break;
    case 1:
      ta = new RainbowFill(lamp);
      break;
    case 2:
      ta = new Fire(lamp);
      break;
    case 99:
      //default case re-sets the animation to first, re-runs
      ta = new iLampAnimation(lamp);
      white();
      break;
    default:
      setAnimation(0);
      //set global animation index to 0
      animation = 0;
      break;
  }
  animation = whichanimation;
  Serial.print("A:");
  Serial.println(animation);
  next_animation = false;
}

String urlRouter(String url) {
  String resp = "";
  // Serial.print("UUU:");
  // Serial.println(url);
  int anim_at = url.indexOf("/anim/");
  Serial.print("Anim@: ");
  Serial.println(anim_at);
  if(anim_at > -1) {
    int anim = url.substring(anim_at+6).toInt();
    setAnimation(anim);
  }
  int color_at = url.indexOf("/color/");
  // Serial.print("Color: ");
  // Serial.println(color_at);

  if(color_at > -1) {
    setAnimation(99);
    //red for now
    lamp->fill_color(0, lamp->getNumLeds()-1, CRGB::Red);
    lamp->render();
  }
  // animation++;
  // setAnimation(animation);
  // resp = "Switched animation to ";
  resp+=animation;
  return resp;
}