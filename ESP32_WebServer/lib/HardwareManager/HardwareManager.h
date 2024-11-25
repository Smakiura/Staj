// HardwareManager.h
#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <Arduino.h>

class HardwareManager {
public:
    HardwareManager(int led1Pin, int led2Pin, int switchPin);
    void initialize(); 
    bool toggleLED1();
    bool toggleLED2();
    bool isSwitchOn();
    String getStatusHTML();
    int getLED1State();
    int getLED2State();
    void led1StateChanged();
    void led2StateChanged();
    void switchStateChanged();

private:
    int _led1Pin;
    int _led2Pin;
    int _switchPin;
    int _led1State; // Track the state of LED1
    int _led2State; // Track the state of LED2
    bool _switchState; // Track switch state
};

#endif // HARDWARE_MANAGER_H
