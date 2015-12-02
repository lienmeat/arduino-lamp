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
    boolean isReady();
    String getFirmware();
    String getAPs();
    boolean joinAP(String ssid, String pass);
    boolean quitAP();
    String getIP();
    boolean setAPParams(String ssid, String pwd, int channel, String encryption);
    int getWifiMode();
    boolean setWifiMode(int mode);
    boolean setupConnection(String type, String ip, int port, int id);
    boolean setupConnection(String type, String ip, int port);
    boolean getCIPMUX();
    boolean setCIPMUX(int mux);
    String getConnectionStatus();
    boolean setServer(int on, int port);
    int getServerTimeout();
    boolean setServerTimeout(int timeout);
    boolean sendResponse(String data, byte connection);
    int getBaud();
    boolean setBaud(int baud);
    void doFirmwareUpgrade();
    String receive(unsigned long maxWaitTime);
    boolean inResponse(String find, unsigned long maxWaitTime);
    int waitUntilAvailable(unsigned long maxWaitTime);
    String waitForResponse(String reponse, unsigned long maxWaitTime);
  protected:
  private:
};

#endif // ESP8266AT_H