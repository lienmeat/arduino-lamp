#ifndef ESP8266AT_H
#define ESP8266AT_H

#include "Arduino.h"
#include <SoftwareSerial.h>

/**
 * Class to handle software-serial based AT command
 * communication with the ESP8266
 */

class ESP8266AT : public SoftwareSerial
{
  public:
    ESP8266AT(int rx_pin, int tx_pin);
    void send(String cmd);
    String receive();
    void reboot();
    int isReady();
    String getFirmware();
    String getAPs();
    int joinAP(String ssid, String pass);
    int quitAP();
    String getIP();
    int setAPParams(String ssid, String pwd, int channel, String encryption);
    int getWifiMode();
    int setWifiMode(int mode);
    int setupConnection(String type, String ip, int port, int id);
    int setupConnection(String type, String ip, int port);
    int getCIPMUX();
    int setCIPMUX(int mux);
    String getConnectionStatus();
    int setServer(int on, int port);
    int getServerTimeout();
    int setServerTimeout(int timeout);
    boolean sendResponse(String data, byte connection);
    int getBaud();
    int setBaud(int baud);
    void doFirmwareUpgrade();
    String receive(unsigned long maxWaitTime);
    int inResponse(String find, unsigned long maxWaitTime);
    int waitUntilAvailable(unsigned long maxWaitTime);
    String waitForResponse(String reponse, unsigned long maxWaitTime);
  protected:
  private:
};

#endif // ESP8266AT_H