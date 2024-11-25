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
    void beginAP(const char* ssid, const char* password);
    void updateWebPage();
    void broadcastUpdate(const String& message);

private:
    AsyncWebServer server;
    AsyncWebSocket ws;
    HardwareManager* hardwareManager;
    std::vector<AsyncWebSocketClient*> clients;
    
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void setupRoutes();
};

#endif // WEB_SERVER_MANAGER_H
