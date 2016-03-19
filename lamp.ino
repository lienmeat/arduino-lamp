#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>


#include "Lamp.h"
#include "animations.h"
#include <SoftwareSerial.h>

//ESP8266
#define ESP_BAUD 9600
#define ESPRX_PIN 3
#define ESPTX_PIN 2
//comms via software serial
SoftwareSerial esp(ESPRX_PIN, ESPTX_PIN);

// lamp/LED details
//led control pin
#define LED_DATA_PIN 4
#define NUM_LEDS 50
//#define BRIGHTNESS  64
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

#define MatrixWidth 4
#define MatrixHeight 10

//Fastled led array
CRGB leds[NUM_LEDS];

//Class to help control the lamp
Lamp* lamp;

//lamp animation abstraction
iLampAnimation* ta;

void setup() {
  Serial.begin(115200);
  esp.begin(ESP_BAUD);
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  lamp = new Lamp(leds, NUM_LEDS, MatrixWidth);
  //go to white animation
  setAnimation(99);
}



void loop() {
  //Serial.println(animation);
  //itterate whatever animation is set
  int animation_delay = ta->itterate();
  int delayed = 0;

  //hack to allow delay between
  //animation itterations while still being able to process
  //api commands
  while(delayed <= animation_delay) {
    process_api_request();
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
}

boolean process_api_request() {
  if(esp.available()) {
    String resp = esp.readString();
//    Serial.print("incoming: ");
//    Serial.println(resp);
    urlRouter(resp);
  }
  return true;
}

String urlRouter(String url) {
//  Serial.print("url:" );
//  Serial.println(url);
  String resp = "OK";
  //all api endpoints start with /lamp/
  if(url.indexOf("/lamp/") > -1) {
    int anim_at = url.indexOf("/lamp/anim/");
    int color_at = url.indexOf("/lamp/color/");
    int set_at = url.indexOf("/lamp/setindiv/");

    //switch to an animation
    if(anim_at > -1) {
      int anim = url.substring(anim_at+11).toInt();
      setAnimation(anim);
    }
    //fill with a color
    else if(color_at > -1) {
      String ct = url.substring(color_at+12);
//      Serial.print("color:");
//      Serial.println(ct);
      CRGB color = parseRGB(ct);
      //Serial.println(color);
      lamp->fill_color(0, lamp->getNumLeds()-1, color);
      lamp->render();
    }
    //set an individual led
    else if(set_at > -1) {
      //set all leds according to semi-colon separated values, rgb values are coma-separated
      parseSetLED(url.substring(set_at+15));
      lamp->render();
    }
  }
  return resp;
}

//parse a color from the url
//format: r,g,b
CRGB parseRGB(String color) {
  int separators[2] = {};
  uint8_t offset = 0;
  for(uint8_t i = 0; i < 2; i++) {
    separators[i] = color.indexOf(",", offset);
    offset = separators[i]+1;
//    Serial.print("Separator at: ");
//    Serial.println(separators[i]);
  }
  if(separators[0] && separators[1]) {
//    Serial.print("parsed: ");
//    Serial.println(color.substring(0, separators[0]).toInt());
//    //color.substring(separators[0]+1, separators[1]).toInt(), color.substring(separators[1]+1).toInt()
//    Serial.println(color.substring(separators[0]+1, separators[1]).toInt());
//    Serial.println(color.substring(separators[1]+1));
    return CRGB(color.substring(0, separators[0]).toInt(), color.substring(separators[0]+1, separators[1]).toInt(), color.substring(separators[1]+1).toInt());
  }
  else{
    //return black
    return CRGB(0, 0, 0);
  }
}

//parse a single led value from url
//format: <led#>,r,g,b
void parseSetLED(String input) {
  uint8_t leds = lamp->getNumLeds() - 1;
  uint8_t offset = 0;
  offset = input.indexOf(",");
  if(offset > -1) {
    lamp->setLed(input.substring(0, offset).toInt(), parseRGB(input.substring(offset+1)));
  }
}
