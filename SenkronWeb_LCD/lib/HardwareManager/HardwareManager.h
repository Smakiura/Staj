#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include <Arduino.h>

class HardwareManager {
public:
    // Constructor
    HardwareManager(int led1Pin, int led2Pin, int switchPin, int joystickUpPin, int joystickDownPin, int joystickLeftPin, int joystickRightPin);
    

    // Initialize pins and set default states for LEDs
    void initialize();

    // Toggle LEDs
    bool toggleLED1();
    bool toggleLED2();

    // Turn LEDs on or off
    void turnLED1On();
    void turnLED1Off();
    void turnLED2On();
    void turnLED2Off();

    // Get states
    bool getLED1State();
    bool getLED2State();
    bool isSwitchOn();
    bool hasStateChanged();

    bool isJoystickUpPressed();
    bool isJoystickDownPressed();
    bool isJoystickLeftPressed();
    bool isJoystickRightPressed();

    String getStatusHTML();
    String getStateUpdate();
    int _led1Pin;   // Pin for LED1
    int _led2Pin;   // Pin for LED2
    int _switchPin; // Pin for the switch


private:
    
    int _joystickUpPin;
    int _joystickDownPin;
    int _joystickLeftPin;
    int _joystickRightPin;

    bool LED1State; // Internal state for LED1
    bool LED2State; // Internal state for LED2
};

#endif
