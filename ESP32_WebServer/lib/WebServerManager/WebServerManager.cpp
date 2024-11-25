#include "WebServerManager.h"

WebServerManager::WebServerManager(HardwareManager* hardwareManager) 
    : server(80), hardwareManager(hardwareManager) {}

void WebServerManager::begin(const char* ssid, const char* password) {
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    //Print the ip address to serial monitor
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Start the server
    setupRoutes();
    server.begin();
    Serial.println("HTTP server started");

}

void WebServerManager::setupRoutes() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/html", hardwareManager->getStatusHTML());
    });

    server.on("/toggleLED1", HTTP_POST, [this](AsyncWebServerRequest *request) {
        hardwareManager->toggleLED1();
        request->redirect("/");
    });

    server.on("/toggleLED2", HTTP_POST, [this](AsyncWebServerRequest *request) {
        hardwareManager->toggleLED2();
        request->redirect("/");
    });

    server.on("/checkSwitch", HTTP_GET, [this](AsyncWebServerRequest *request) {
        static bool lastSwitchState = HIGH;
        bool currentSwitchState = hardwareManager->isSwitchOn();
        String jsonResponse = "{\"switchStateChanged\": " + String(currentSwitchState != lastSwitchState ? "true" : "false") + "}";
        if (currentSwitchState != lastSwitchState) {
            lastSwitchState = currentSwitchState;
        }
        request->send(200, "application/json", jsonResponse);
    });

    server.on("/on.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/on.png", "image/png");
    });

    server.on("/off.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/off.png", "image/png");
    });
}
void WebServerManager::update() {
    
}