#include "HardwareManager.h"

HardwareManager::HardwareManager(int led1Pin, int led2Pin, int switchPin, int joystickUpPin, int joystickDownPin, int joystickLeftPin, int joystickRightPin) 
    : _led1Pin(led1Pin), _led2Pin(led2Pin),_switchPin(switchPin), _joystickUpPin(joystickUpPin), _joystickDownPin(joystickDownPin), _joystickLeftPin(joystickLeftPin), _joystickRightPin(joystickRightPin) {}
  
    bool lastLED1State = false;
    bool lastLED2State = false;
    bool lastSwitchState = HIGH;


void HardwareManager::initialize() {
    pinMode(_led1Pin, OUTPUT);
    pinMode(_led2Pin, OUTPUT);
    pinMode(_switchPin, INPUT_PULLUP);
    pinMode(_joystickUpPin, INPUT_PULLUP);
    pinMode(_joystickDownPin, INPUT_PULLUP);
    pinMode(_joystickLeftPin, INPUT_PULLUP);
    pinMode(_joystickRightPin, INPUT_PULLUP);

    // Initialize LEDs to OFF
    digitalWrite(_led1Pin, LOW);
    digitalWrite(_led2Pin, LOW);
    
}

void HardwareManager::turnLED1On() {
    digitalWrite(_led1Pin, HIGH);
}

void HardwareManager::turnLED1Off() {
    digitalWrite(_led1Pin, LOW);
}

void HardwareManager::turnLED2On() {
    digitalWrite(_led2Pin, HIGH);
}

void HardwareManager::turnLED2Off() {
    digitalWrite(_led2Pin, LOW);
}

bool HardwareManager::getLED1State() {
    return digitalRead(_led1Pin) == HIGH;
}

bool HardwareManager::getLED2State() {
    return digitalRead(_led2Pin) == HIGH;
}

bool HardwareManager::isJoystickUpPressed() {
    return digitalRead(_joystickUpPin) == LOW;
}

bool HardwareManager::isJoystickDownPressed() {
    return digitalRead(_joystickDownPin) == LOW;
}

bool HardwareManager::isJoystickLeftPressed() {
    return digitalRead(_joystickLeftPin) == LOW;
}

bool HardwareManager::isJoystickRightPressed() {
    return digitalRead(_joystickRightPin) == LOW;
}

bool HardwareManager::isSwitchOn() {
    return digitalRead(_switchPin) == LOW;  // Assuming LOW means switch is on
}

bool HardwareManager::hasStateChanged() {
    bool currentLED1State = getLED1State();
    bool currentLED2State = getLED2State();
    bool currentSwitchState = isSwitchOn();

    // Check if any state has changed compared to last known state
    bool stateChanged = (currentLED1State != lastLED1State) || (currentLED2State != lastLED2State) || (currentSwitchState != lastSwitchState);

    // Update last known states if there was a change
    if (stateChanged) {
        lastLED1State = currentLED1State;
        lastLED2State = currentLED2State;
        lastSwitchState = currentSwitchState;
    }

    return stateChanged;
}

bool HardwareManager::toggleLED1() {
    int ledState = digitalRead(_led1Pin);
    digitalWrite(_led1Pin, !ledState);
    return !ledState;
}

bool HardwareManager::toggleLED2() {
    int ledState = digitalRead(_led2Pin);
    digitalWrite(_led2Pin, !ledState);
    return !ledState;
}

String HardwareManager::getStateUpdate() {
    String stateUpdate = "{\"LED1\": " + String(getLED1State() ? "true" : "false") + 
                         ", \"LED2\": " + String(getLED2State() ? "true" : "false") + 
                         ", \"switch\": " + String(isSwitchOn() ? "true" : "false") + "}";
    return stateUpdate;
}


String HardwareManager::getStatusHTML() {
    String html = "<html><head>";
    html += "<style>body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: Arial, sans-serif; background-color: #f0f0f0; }";
    html += ".container { background-color: white; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); text-align: center; width: 300px; }";
    html += ".indicator { display: inline-block; width: 20px; height: 20px; border-radius: 50%; margin-right: 10px; }";
    html += ".led1 { background-color: green; } .led2 { background-color: red; } .off { background-color: gray; }";
    html += "</style>";
    
    // JavaScript to periodically check the state of LEDs and the switch
    html += "<script>";
    html += "function checkState() {";
    html += "    fetch('/checkState').then(response => response.json()).then(data => {";
    html += "        if (data.stateChanged) {";
    html += "            location.reload();";  // Reload the page if there is a change
    html += "        }";
    html += "    });";
    html += "    setTimeout(checkState, 1000);";  // Check every second
    html += "}";
    html += "window.onload = checkState;";
    html += "</script>";
    
    html += "</head>";
    html += "<body><div class='container'><h1>LED Status</h1>";
    html += "<div><span>LED1: </span><span class='indicator " + String(getLED1State() ? "led1" : "off") + "'></span>";
    html += "<form action=\"/toggleLED1\" method=\"POST\" style='display:inline;'><input type=\"submit\" value=\"Toggle LED1\"></form></div>";
    html += "<div><span>LED2: </span><span class='indicator " + String(getLED2State() ? "led2" : "off") + "'></span>";
    html += isSwitchOn() ? "<form action=\"/toggleLED2\" method=\"POST\" style='display:inline;'><input type=\"submit\" value=\"Toggle LED2\"></form>" 
                         : "<button disabled>Toggle LED2 (Switch OFF)</button>";
    html += "</div><div><span>Switch Status: </span>";
    html += "<img src='" + String(isSwitchOn() ? "/off.png" : "/on.png") + "' style='width: 50px; height: 20px;' /></div></div></body></html>";
    return html;
}
