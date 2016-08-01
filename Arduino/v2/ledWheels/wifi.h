#ifndef WIFI
#define WIFI

#include "settings.h"

ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

void wifi_init(settingList* settings)
{
  if(settings->wifiAPMode)
  {
      Serial.print("AP_MODE:");
      Serial.print(settings->wifiESSID);
      Serial.print(" - ");
      Serial.println(settings->wifiPasswd);
      WiFi.disconnect();
      yield();
      WiFi.softAP(settings->wifiESSID, settings->wifiPasswd);
      yield();
  } else {
      Serial.print("WIFI_CLIENT_MODE:");
      Serial.print(settings->wifiESSID);
      Serial.print(" - ");
      Serial.println(settings->wifiPasswd);
      WiFi.softAPdisconnect(true);
      yield();
      WiFi.mode(WIFI_STA);
      WiFi.begin(settings->wifiESSID, settings->wifiPasswd);
      yield();
  }

    MDNS.begin(settings->id);
    server.on("/", HTTP_GET, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if(!Update.begin(maxSketchSpace)){//start with max available size
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser\n", settings->id);
}


#endif // WIFI

