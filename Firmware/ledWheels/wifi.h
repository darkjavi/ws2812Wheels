#ifndef WIFI
#define WIFI

#include "settings.h"
#include "gyro.h"

ESP8266WebServer httpServer(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

WiFiServer TCPserver(31415);
#define MAX_SRV_CLIENTS 3
WiFiClient serverClients[MAX_SRV_CLIENTS];


void parseCommand(String* data)
{

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

void updateClients()
{
    uint8_t i;
    //check if there are any new clients
    if (TCPserver.hasClient()){
      for(i = 0; i < MAX_SRV_CLIENTS; i++){
        //find free/disconnected spot
        if (!serverClients[i] || !serverClients[i].connected()){
          if(serverClients[i]) serverClients[i].stop();
          serverClients[i] = TCPserver.available();
          Serial.print("New client: "); Serial.println(i);
          continue;
        }
      }
      //no free/disconnected spot so reject
      WiFiClient serverClient = TCPserver.available();
      serverClient.stop();
    }
    //check clients for data
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        if(serverClients[i].available()){
          String data;
          while(serverClients[i].available())
          {
              data += serverClients[i].read();
          }
          parseCommand(&data);
        }
      }
    }
}

void sendAccRead(MPU6050* g)
{
    String str;
    str += "AcX:";
    str += g->AcX;
    str += "|";
    str += "AcY:";
    str += g->AcY;
    str += "|";
    str += "AcZ:";
    str += g->AcZ;
    str += "|";
    str += "GyX:";
    str += g->GyX;
    str += "|";
    str += "GyY:";
    str += g->GyY;
    str += "|";
    str += "GyZ:";
    str += g->GyZ;
    str += "|";
    str += "AcX:";
    str += g->Temp;
    str += "\n";

    for(int i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected())
      {
        serverClients[i].print(str);
      }
    }
}


#endif // WIFI

