#include <Arduino.h>
#include "HardwareManager.h"
#include "WebServerManager.h"
#include "LCDManager.h"  // Include the LCDManager class

// Constants for screen dimensions and pins
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 128;
const int JOYSTICK_LEFT_PIN = 22;   // Joystick left direction
const int JOYSTICK_RIGHT_PIN = 21;  // Joystick right direction
const int JOYSTICK_UP_PIN = 4;      // Joystick up direction
const int JOYSTICK_DOWN_PIN = 25;   // Joystick down direction
const int BUTTON_PIN = 33;          // Joystick center button (KEY1)
const int LED1_PIN = 26;
const int LED2_PIN = 27;
const int SWITCH_PIN = 23;          // Switch is now on pin 23

// Create hardware, server, and LCD manager objects
HardwareManager hardwareManager(LED1_PIN, LED2_PIN, SWITCH_PIN, JOYSTICK_UP_PIN, JOYSTICK_DOWN_PIN, JOYSTICK_LEFT_PIN, JOYSTICK_RIGHT_PIN);
WebServerManager webServerManager(&hardwareManager);
LCDManager lcdManager(&hardwareManager);


void setup() {
    Serial.begin(9600);

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // Set joystick pins as inputs with pull-up resistors
    pinMode(JOYSTICK_LEFT_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_RIGHT_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_UP_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize hardware
    hardwareManager.initialize();

    // Initialize LCD
    lcdManager.begin();

    // Start the web server
    webServerManager.beginAP("esp32_Router", "123456789");
    //webServerManager.begin("sensor-wifi", "sensorlab2017");

    // Display initial status on LCD
    lcdManager.drawLED1Status();

    
}

void loop() {
    

    lcdManager.update();
    
   if (hardwareManager.hasStateChanged()) {
    // Get the updated state as a JSON string
    String stateUpdate = hardwareManager.getStateUpdate();

    // Broadcast the update to all WebSocket clients
    webServerManager.broadcastUpdate(stateUpdate);
}
}

