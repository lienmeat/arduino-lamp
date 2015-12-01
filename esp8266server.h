#ifndef ESP8266SERVER_H
#define ESP8266SERVER_H

#include "Lamp.h"

typedef String (*UrlHandlerCallback)(String);

void esp8266ServerSetup(Lamp* lamp);

void readyUp();

void connectWifi();

void startServer();

boolean WiFiconnected();

boolean serverStarted();

boolean hasIP();

void send(String msg);
void received(String msg);
void debug(String msg);

boolean send_response(String message, int connection);

boolean process_request(UrlHandlerCallback callback);

String getUrl(String headers);

#endif // ESP8266SERVER_H