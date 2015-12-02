#include "esp8266at.h"

ESP8266AT::ESP8266AT(int rx, int tx) : SoftwareSerial(rx, tx){ }

void ESP8266AT::send(String cmd) {
	println(cmd);
	// Serial.println("SENT> " +cmd);
}

void ESP8266AT::reboot() {
	send("AT+RST");
	delay(10000);
	//waitForResponse("OK", 20000);
}

boolean ESP8266AT::isReady() {
	send("AT");
	return inResponse("OK", 1000);
}


String ESP8266AT::getFirmware() {
	return "Not Yet implemented";
}

String ESP8266AT::getAPs() {
	return "Not Yet implemented";
}

boolean ESP8266AT::joinAP(String ssid, String pass) {
	send("AT+CWJAP=\"" + ssid + "\",\"" + pass + "\"");
	if(waitForResponse("WIFI GOT IP", 10000) != "") {
		return true;
	}
	return false;
}

boolean ESP8266AT::quitAP() {
	//todo:
	return true;
}

boolean ESP8266AT::setAPParams(String ssid, String pass, int channel, String encryption) {
	//todo:
	return true;
}

int ESP8266AT::getWifiMode() {

	//todo:
	return 0;
}

boolean ESP8266AT::setWifiMode(int mode) {
	String cmd="AT+CWMODE=";
	cmd+=mode;
	send(cmd);
	if(waitForResponse("OK", 10000) != "") {
		return true;
	}
	return false;
}

boolean ESP8266AT::setupConnection(String type, String ip, int port, int id) {
	//todo:
	return true;
}

boolean ESP8266AT::setupConnection(String type, String ip, int port) {
	//todo:
	return true;
}

boolean ESP8266AT::getCIPMUX() {
	String cmd = "AT+CIPMUX";
	send(cmd);
	return inResponse("OK", 5000);
}

boolean ESP8266AT::setCIPMUX(int mux) {
	String cmd = "AT+CIPMUX=";
	cmd += mux;
	send(cmd);
	return inResponse("OK", 5000);
}

String ESP8266AT::getConnectionStatus() {
	//todo:
	return "Not Yet implemented";
}

boolean ESP8266AT::setServer(int on, int port) {
	String cmd = "AT+CIPSERVER=";
	cmd+=on;
	cmd+=",";
	cmd+=port;
	send(cmd);
	if(waitForResponse("OK", 20000) != "") {
		return true;
	}
	return false;
}

int ESP8266AT::getServerTimeout() {
	//todo:
	return 0;
}

boolean ESP8266AT::setServerTimeout(int timeout) {
	//todo:
	return true;
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
	String resp = receive(3000);
	if(resp.indexOf(">") != -1) {
		send(data);
		String resp2 = waitForResponse("SEND OK", 3000);
		if(resp2 != "") {
			ret = true;
			cmd = "AT+CIPCLOSE=";
			cmd += connection;
			send(cmd);
		}
	}
	else if(resp.indexOf("ERROR") != -1) {
		ret = false;
	}
	return ret;
}

String ESP8266AT::receive(unsigned long maxWaitTime) {
	String response;
	int available = waitUntilAvailable(maxWaitTime);
	while(this->available()) {
		response += readString();
		//Serial.println("RECV> " + response);
	}
	return response;
}

boolean ESP8266AT::inResponse(String find, unsigned long maxWaitTime) {
	String res = receive(maxWaitTime);
	if(res.indexOf(find) != -1) {
		return true;
	}
	return false;
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
  String resp;
  unsigned long startTime;
  int c = 0;
  int found = 0;
  startTime = millis();
  do {
	c = available();
	if (c) {
		resp = readString();
		if(resp.indexOf(reponse) > -1) {
			found = 1;
			// Serial.println("RECV> " + resp);
			return resp;
			break;
		}
	}
    
  } while(millis() - startTime < maxWaitTime);
  // Serial.println("RECV> " + resp);
  return "";
}