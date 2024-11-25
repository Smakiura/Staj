// HardwareManager.cpp
#include "HardwareManager.h"

HardwareManager::HardwareManager(int led1Pin, int led2Pin, int switchPin)
    : _led1Pin(led1Pin), _led2Pin(led2Pin), _switchPin(switchPin), _led1State(LOW), _led2State(LOW), _switchState(HIGH) {}

void HardwareManager::initialize() {
    pinMode(_led1Pin, OUTPUT);
    pinMode(_led2Pin, OUTPUT);
    pinMode(_switchPin, INPUT_PULLUP);

    // Initialize LEDs to OFF
    digitalWrite(_led1Pin, LOW);
    digitalWrite(_led2Pin, LOW);
    _led1State = LOW;
    _led2State = LOW;
}

bool HardwareManager::toggleLED1() {
    _led1State = !_led1State;
    digitalWrite(_led1Pin, _led1State);
    return _led1State;
}

bool HardwareManager::toggleLED2() {
    _led2State = !_led2State;
    digitalWrite(_led2Pin, _led2State);
    return _led2State;
}

bool HardwareManager::isSwitchOn() {
    _switchState = digitalRead(_switchPin) == LOW;
    return _switchState;
}

String HardwareManager::getStatusHTML() {
    String html = "<html><head><style>body { font-family: Arial, sans-serif; }</style></head><body>";
    html += "<h1>LED1: <span style='color:" + String(_led1State == HIGH ? "green" : "gray") + "'>●</span></h1>";
    html += "<h1>LED2: <span style='color:" + String(_led2State == HIGH ? "red" : "gray") + "'>●</span></h1>";
    html += "<h1>Switch: " + String(_switchState ? "ON" : "OFF") + "</h1>";
    html += "<form action='/toggleLED1' method='POST'><input type='submit' value='Toggle LED1'></form><br>";
    html += "<form action='/toggleLED2' method='POST'><input type='submit' value='Toggle LED2'></form><br>";
    html += "</body></html>";
    return html;
}

int HardwareManager::getLED1State() {
    return _led1State;
}

int HardwareManager::getLED2State() {
    return _led2State;
}

void HardwareManager::led1StateChanged() {
    // Notify LCDManager to update display
}

void HardwareManager::led2StateChanged() {
    // Notify LCDManager to update display
}

void HardwareManager::switchStateChanged() {
    // Notify LCDManager to update display
}
