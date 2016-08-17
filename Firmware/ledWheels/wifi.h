#ifndef WIFI
#define WIFI

#include "settings.h"
#include "gyro.h"

ESP8266WebServer httpServer(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

WiFiServer TCPserver(31415);
#define MAX_SRV_CLIENTS 3
WiFiClient serverClients[MAX_SRV_CLIENTS];

static std::vector<String> splitStr(String& str,String sep)
{
        std::vector<String> result;
  if(str.length() < 1) return result;
  int sep_index = str.indexOf(sep);
  if(sep_index == -1)
  {//no hay separador, se devuelve un unico elemento
    result.push_back(str);
    return result;
  }

  while (sep_index >= 0)
  {
    result.push_back(str.substring(0,sep_index));
    str = str.substring(sep_index+sep.length());
    sep_index = str.indexOf(sep);
  }
  if(str.length()) result.push_back(str);
        return result;
}


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
    httpServer.on("/", HTTP_GET, [](){
      httpServer.sendHeader("Connection", "close");
      httpServer.sendHeader("Access-Control-Allow-Origin", "*");
      httpServer.send(200, "text/html", serverIndex);
    });
    httpServer.on("/update", HTTP_POST, [](){
      httpServer.sendHeader("Connection", "close");
      httpServer.sendHeader("Access-Control-Allow-Origin", "*");
      httpServer.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
      HTTPUpload& upload = httpServer.upload();
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
    httpServer.begin();
    TCPserver.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser to upgrade\n", settings->id);
}

void manageSocketClients()
{
    //find free/disconnected spot
    for(uint8_t i = 0; i < MAX_SRV_CLIENTS; i++){
        if(serverClients[i] && !serverClients[i].connected())
        {
            serverClients[i].stop();
            Serial.print("Client disconnected"); Serial.println(i);
        }
    }
    //check if there are any new clients
    if (TCPserver.hasClient()){
      for(uint8_t i = 0; i < MAX_SRV_CLIENTS; i++){
        //find free/disconnected spot
        if (!serverClients[i] || !serverClients[i].connected())
        {
          if(serverClients[i])
          {
              serverClients[i].stop();
              Serial.print("Client disconnected"); Serial.println(i);
          }
          serverClients[i] = TCPserver.available();
          Serial.print("New client: "); Serial.println(i);
          break;
        }
      }
      //no free/disconnected spot so reject
      WiFiClient serverClient = TCPserver.available();
      serverClient.stop();
    }
}
#endif // WIFI

