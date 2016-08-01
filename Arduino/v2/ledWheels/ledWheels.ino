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
ws2812Strip   myLedStrip(&myEEPROM.settings());
MPU6050       myGyro;
ledWheel      myLedWheel(myLedStrip.getLeds(),&myGyro);



void setup()
{
  Serial.begin(115200);
  Serial.println("Init...");
  yield();
  wifi_init(&myEEPROM.settings());
  yield();
  myGyro.init();
  myLedStrip.setup();
  Serial.println("...Ready!");
  yield();
}

void loop() {
  server.handleClient();
  myGyro.fullRead();
  //myGyro.dumpRead();
  myLedWheel.animate();
  myLedStrip.update();
  delay(10);
}
