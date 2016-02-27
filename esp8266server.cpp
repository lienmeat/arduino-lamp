#include <SoftwareSerial.h>
#include "esp8266server.h"
#include "esp8266at.h"
#include "FastLED.h"
#include "Lamp.h"
#define SSID "The Ping in the North"
#define PASS "eatyourvegetables"

//startup status LEDS
byte ESP8266READY = 16;
byte ESP8266CONNECTED = 32;
byte ESP8266SERVERSTARTED = 48;

boolean ready = false;
boolean connected = false;
boolean server_started = false;

ESP8266AT* espat;

Lamp* glamp;

void esp8266ServerSetup(Lamp* lamp)
{
	
	espat = new ESP8266AT(4, 3);
	espat->begin(9600);
	glamp = lamp;
	glamp->fill_color(0, ESP8266READY, CRGB::Red);
	glamp->fill_color(ESP8266READY+1, ESP8266CONNECTED, CRGB::Red);
	glamp->fill_color(ESP8266CONNECTED+1, ESP8266SERVERSTARTED, CRGB::Red);
	glamp->render();
	//wait till board is ready
	readyUp();
	//connect to the wifi
	connectWifi();
	//start up server
	startServer();
}

void readyUp() {
	//reset the board
	espat->reboot();
	while(!ready) {
		ready = espat->isReady();
	}
	glamp->fill_color(0, ESP8266READY, CRGB::Green);
	//glamp->setLed(ESP8266READY, CRGB::Green);
	glamp->render();
}

void connectWifi() {
	espat->setWifiMode(1);
	while(!connected)
	{
		if(WiFiconnected())
		{
			glamp->fill_color(ESP8266READY+1, ESP8266CONNECTED, CRGB::Green);
			//glamp->setLed(ESP8266CONNECTED, CRGB::Green);
			glamp->render();
			connected = true;
			break;
		}
	}
}

void startServer() {
	while(!server_started)
	{
		if(serverStarted())
		{
			glamp->fill_color(ESP8266CONNECTED+1, ESP8266SERVERSTARTED, CRGB::Green);
			// glamp->setLed(ESP8266SERVERSTARTED, CRGB::Green);
			glamp->render();
			server_started = true;
			break;
		}
	}
}

boolean WiFiconnected() 
{
	if(hasIP()) {
		return true;
	}
	else{ 
		return espat->joinAP(SSID, PASS);
	}
}

boolean serverStarted() {
	if(espat->setCIPMUX(1)) {
		return espat->setServer(1, 80);
	}
	return false;
}

boolean hasIP()
{
	String ip = espat->getIP();
	return !(ip == "" || ip == "0.0.0.0");
}

void debug(String msg) {
	Serial.println(msg);
}


boolean send_response(String message, byte connection) {
	// glamp->fill_color(0, 10, CRGB::Blue);
	// glamp->render();
	boolean ret = espat->sendResponse(message, connection);
	// glamp->fill_color(11, 20, CRGB::Green);
	// glamp->render();
	return ret;
}

boolean process_request(UrlHandlerCallback callback) {
	byte cid = 0;
	String response = "NOPE";
	if( espat->available()) {
		String request = espat->readString();
		int ipd_idx = request.indexOf("+IPD,");
		if(ipd_idx > -1) {
			Serial.println(request);
			// glamp->fill_color(21, 30, CRGB::Red);
			// glamp->render();
			int cid_end = request.indexOf(",", ipd_idx+5);
			cid = request.substring(ipd_idx+5,cid_end).toInt();
			String url = getUrl(request);
			String callback_resp = callback(url);
			if(callback_resp != "") {
				response = callback_resp;
				return send_response(response, cid);
			}
			
		}
		
	}
	return false;
}

String getUrl(String headers) {
	int url_idx = headers.indexOf(":GET /")+5;
	int end_url_idx = headers.indexOf("HTTP/", url_idx)-1;
	String url = headers.substring(url_idx, end_url_idx);
	return url;
}
