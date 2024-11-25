#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include "HardwareManager.h"
#include <TFT_eSPI.h>  // Assuming you're using TFT_eSPI library for the display

class LCDManager {
public:
    LCDManager(HardwareManager* hardwareManager);
    
    void begin();  // Initialize the display
    void update(); // Update the screen
    void setJoystickState(bool joystickLeft, bool joystickRight, bool joystickUp, bool joystickDown);
    void drawLED1Status();
    
private:
    void drawScreen();
    void drawLED2Status();

    HardwareManager* hardwareManager;
    int currentScreen;
    unsigned long lastUpdateTime;
    unsigned long joystickDelay;
    unsigned long lastJoystickUpdateTime;
    // Variables to track last states for detecting changes
    bool lastLED1State = false;
    bool lastLED2State = false;
    bool lastSwitchState = false;
    
    TFT_eSPI tft;  // TFT display object
};

#endif
