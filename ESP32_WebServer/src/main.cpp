// main.cpp
#include <Arduino.h>
#include "HardwareManager.h"
#include "WebServerManager.h"
#include "LCDManager.h"

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 128;
const int JOYSTICK_LEFT_PIN = 22;   
const int JOYSTICK_RIGHT_PIN = 21;  
const int JOYSTICK_UP_PIN = 4;      
const int JOYSTICK_DOWN_PIN = 25;   
const int BUTTON_PIN = 33;          
const int LED1 = 26;
const int LED2 = 27;
const int Switch = 23;

// Create hardware, server, and LCD manager objects
HardwareManager hardwareManager(LED1, LED2, Switch);  
WebServerManager webServerManager(&hardwareManager);
LCDManager lcdManager(&hardwareManager);

void setup() {
    Serial.begin(9600);

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    pinMode(JOYSTICK_LEFT_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_RIGHT_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_UP_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    hardwareManager.initialize();
    lcdManager.begin();

    webServerManager.begin("sensor-wifi", "sensorlab2017"); // Start the web server
}

void loop() {
    // Check for joystick inputs to switch between screens
    if (digitalRead(JOYSTICK_LEFT_PIN) == LOW) {
        lcdManager.nextScreen();
        delay(300);  // Debounce delay
    }
    if (digitalRead(JOYSTICK_RIGHT_PIN) == LOW) {
        lcdManager.nextScreen();
        delay(300);  // Debounce delay
    }

    // Update LCD based on hardware state
    lcdManager.update();

    // Web server is handled asynchronously; no need for explicit calls
}
