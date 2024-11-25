#include "WebServerManager.h"
#include "SPIFFS.h"

// Include WebSocket library
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

WebServerManager::WebServerManager(HardwareManager* hardwareManager) 
    : server(80), ws("/ws"), hardwareManager(hardwareManager) {}

void WebServerManager::begin(const char* ssid, const char* password) {
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    // Print the IP address to the serial monitor
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // Start the server
    setupRoutes();
    server.begin();

    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
        onWsEvent(server, client, type, arg, data, len);
    });
    server.addHandler(&ws);


    Serial.println("HTTP and WebSocket server started");
}

void WebServerManager::beginAP(const char* ssid, const char* password) {
    // Start the ESP32 as an Access Point
    WiFi.softAP(ssid, password); // Set SSID and password for AP
    Serial.print("Setting up Access Point...");
    
    delay(1000);
    
    Serial.println("\nAccess Point Started");

    // Print the IP address of the Access Point
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // Start the server
    setupRoutes();
    server.begin();

    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
        onWsEvent(server, client, type, arg, data, len);
    });
    server.addHandler(&ws);

    Serial.println("HTTP server started");
}

void WebServerManager::setupRoutes() {
    // WebSocket event handler
    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
        onWsEvent(server, client, type, arg, data, len);
    });

    // Add WebSocket to the server
    server.addHandler(&ws);
    
    // Route for the main page
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/html", hardwareManager->getStatusHTML());
    });

    server.on("/broadcastUpdate", HTTP_POST, [this](AsyncWebServerRequest* request) {
        // Example: Trigger a broadcast update when requested
        this->broadcastUpdate("Update message to all clients");
        request->send(200, "text/plain", "Update broadcasted");
    });

    // Toggle LED1
    server.on("/toggleLED1", HTTP_POST, [this](AsyncWebServerRequest *request) {
        hardwareManager->toggleLED1();
        String message = hardwareManager->_led1Pin ? "LED1 is ON" : "LED1 is OFF"; 
        broadcastUpdate(message);  // Notify clients of state change
        request->redirect("/");
    });
    

    // Toggle LED2
    server.on("/toggleLED2", HTTP_POST, [this](AsyncWebServerRequest *request) {
        hardwareManager->toggleLED2();
        String message = hardwareManager->_led2Pin ? "LED2 is ON" : "LED2 is OFF"; 
        broadcastUpdate(message);  // Notify clients of state change
        request->redirect("/");
    });

    // Route to check the state of the LEDs and the switch
    server.on("/checkState", HTTP_GET, [this](AsyncWebServerRequest *request) {
        static bool lastLED1State = false;
        static bool lastLED2State = false;
        static bool lastSwitchState = HIGH;

        bool currentLED1State = hardwareManager->getLED1State();
        bool currentLED2State = hardwareManager->getLED2State();
        bool currentSwitchState = hardwareManager->isSwitchOn();

        bool stateChanged = false;

        // Check if any state has changed
        if (currentLED1State != lastLED1State || currentLED2State != lastLED2State || currentSwitchState != lastSwitchState) {
            stateChanged = true;
                 String updateMessage = "{\"LED1\": " + String(currentLED1State ? "true" : "false") + 
                           ", \"LED2\": " + String(currentLED2State ? "true" : "false") + 
                           ", \"switch\": " + String(currentSwitchState ? "true" : "false") + "}";
                 broadcastUpdate(updateMessage);  // Notify clients of state change
            }

        // Update the last state if there's a change
        if (currentLED1State != lastLED1State) lastLED1State = currentLED1State;
        if (currentLED2State != lastLED2State) lastLED2State = currentLED2State;
        if (currentSwitchState != lastSwitchState) lastSwitchState = currentSwitchState;

        // Return a JSON response indicating whether the state has changed
        String jsonResponse = "{\"stateChanged\": " + String(stateChanged ? "true" : "false") + "}";
        request->send(200, "application/json", jsonResponse);
    });

    // Optional: You can still use the image routes
    server.on("/on.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/on.png", "image/png");
    });

    server.on("/off.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/off.png", "image/png");
    });
    
     
}

void WebServerManager::onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        // Client has connected, add to list of clients
        clients.push_back(client);
    } else if (type == WS_EVT_DISCONNECT) {
        // Client has disconnected, remove from list
        auto it = std::remove(clients.begin(), clients.end(), client);
        clients.erase(it, clients.end());
    }
}


void WebServerManager::broadcastUpdate(const String& stateUpdate) {
    for (AsyncWebSocketClient* client : clients) {
        client->text(stateUpdate);  // Send the update as text to the client
    }
}