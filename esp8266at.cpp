#include "esp8266at.h"

ESP8266AT::ESP8266AT(int rx, int tx) : SoftwareSerial(rx, tx){ }

void ESP8266AT::send(String cmd) {
	println(cmd);
	Serial.println("SENT> " +cmd);
}

void ESP8266AT::reboot() {
	send("AT+RST");
	delay(10000);
	//waitForResponse("OK", 20000);
}

int ESP8266AT::isReady() {
	send("AT");
	return inResponse("OK", 1000);
}


String ESP8266AT::getFirmware() {
	return "Not Yet implemented";
}

String ESP8266AT::getAPs() {
	return "Not Yet implemented";
}

int ESP8266AT::joinAP(String ssid, String pass) {
	send("AT+CWJAP=\"" + ssid + "\",\"" + pass + "\"");
	if(waitForResponse("WIFI GOT IP", 10000) != "") {
		return 1;
	}
	return 0;
}

int ESP8266AT::quitAP() {
	//todo:
	return 1;
}

int ESP8266AT::setAPParams(String ssid, String pass, int channel, String encryption) {
	//todo:
	return 1;
}

int ESP8266AT::getWifiMode() {

	//todo:
	return 0;
}

int ESP8266AT::setWifiMode(int mode) {
	String cmd="AT+CWMODE=";
	cmd+=mode;
	send(cmd);
	if(waitForResponse("OK", 10000) != "") {
		return 1;
	}
	return 0;
}

int ESP8266AT::setupConnection(String type, String ip, int port, int id) {
	//todo:
	return 0;
}

int ESP8266AT::setupConnection(String type, String ip, int port) {
	//todo:
	return 0;
}

int ESP8266AT::getCIPMUX() {
	String cmd = "AT+CIPMUX";
	send(cmd);
	return inResponse("OK", 5000);
}

int ESP8266AT::setCIPMUX(int mux) {
	String cmd = "AT+CIPMUX=";
	cmd += mux;
	send(cmd);
	return inResponse("OK", 5000);
}

String ESP8266AT::getConnectionStatus() {
	//todo:
	return "Not Yet implemented";
}

int ESP8266AT::setServer(int on, int port) {
	String cmd = "AT+CIPSERVER=";
	cmd+=on;
	cmd+=",";
	cmd+=port;
	send(cmd);
	if(waitForResponse("OK", 20000) != "") {
		return 1;
	}
	return 0;
}

int ESP8266AT::getServerTimeout() {
	//todo:
	return 0;
}

int ESP8266AT::setServerTimeout(int timeout) {
	//todo:
	return 0;
}


String ESP8266AT::getIP() {
	send("AT+CIFSR");
	String ip = "0.0.0.0";
	String resp = waitForResponse("+CIFSR:STAIP,", 10000);
	if(resp != "") {
		int ip_idx = resp.indexOf("+CIFSR:STAIP,\"") + 14;
		int ip_end = resp.indexOf("\"", ip_idx);
		ip = resp.substring(ip_idx, ip_end);
	}
	return ip;
}

void ESP8266AT::doFirmwareUpgrade() {
	//todo:
}

boolean ESP8266AT::sendResponse(String data, byte connection) {
	boolean ret = false;
	String cmd = "AT+CIPSEND=";
	cmd += connection;
	cmd += ",";
	cmd += data.length();
	send(cmd);
	String resp = waitForResponse(">", 5000);
	if(resp != "") {
		send( data);
		String resp2 = waitForResponse("SEND OK", 5000);
		if(resp2 != "") {
			ret = true;
		}
		String cmd = "AT+CIPCLOSE=";
		cmd += connection;
		send(cmd);
		waitForResponse("OK", 5000);
	}
	return ret;
}

String ESP8266AT::receive(unsigned long maxWaitTime) {
	String response;
	int available = waitUntilAvailable(maxWaitTime);
	while(this->available()) {
		response += readString();
		Serial.println("RECV> " + response);
	}
	return response;
}

int ESP8266AT::inResponse(String find, unsigned long maxWaitTime) {
	String res = receive(maxWaitTime);
	if(res.indexOf(find) != -1) {
		return 1;
	}
	return 0;
}

// Waits until data becomes available, or a timeout occurs
int ESP8266AT::waitUntilAvailable(unsigned long maxWaitTime)
{
  unsigned long startTime;
  int c = 0;
  startTime = millis();
  do {
    c = available();
    if (c) break;
  } while(millis() - startTime < maxWaitTime);
  
  return c;
}


// Waits until data becomes available, or a timeout occurs
String ESP8266AT::waitForResponse(String reponse, unsigned long maxWaitTime)
{
  unsigned long startTime;
  int c = 0;
  int found = 0;
  startTime = millis();
  do {
	c = available();
	if (c) {
		String resp = readString();
		Serial.println("RECV> " + resp);
		if(resp.indexOf(reponse) > -1) {
			found = 1;
			return resp;
			break;
		}
	}
    
  } while(millis() - startTime < maxWaitTime);
  
  return "";
}