#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>
#include <vector>
#include <EEPROM.h>


#include "settings.h"
#include "wifi.h"
#include "gyro.h"
#include "sound.h"
#include "ws2812b.h"
#include "ledwheel.h"


EEPROMStorage myEEPROM;
ledWheel      myLedWheel(&myEEPROM);

uint8_t c = 0;


void setup()
{
  Serial.begin(115200);
  Serial.println("Init...");
  yield();
  myLedWheel.setup();
  wifi_init(&myEEPROM.settings());
  yield();
  Serial.println("...Ready!");
  yield();
  //myLedWheel.setLigthOfSpeed();
  myLedWheel.setPositionLights();;
  myLedWheel.flashEffect();
}

void loop() {
  httpServer.handleClient();
  manageSocketClients();
  myLedWheel.update();
  delay(5);
}
