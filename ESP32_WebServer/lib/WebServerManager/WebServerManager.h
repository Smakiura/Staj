#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include "HardwareManager.h"

class WebServerManager {
public:
    WebServerManager(HardwareManager* hardwareManager);
    void begin(const char* ssid, const char* password);
    void update();
private:
    AsyncWebServer server;
    HardwareManager* hardwareManager;
    void setupRoutes();
};

#endif // WEB_SERVER_MANAGER_H
