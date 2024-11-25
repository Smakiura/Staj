// LCDManager.h
#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include "HardwareManager.h"
#include <TFT_eSPI.h> // Include the TFT library

class LCDManager {
public:
    LCDManager(HardwareManager* hardwareManager);
    void begin();
    void update();
    void nextScreen();
    void displayLED1Status();
    void displayLED2Status();
    void updateSwitchStatus();
 

private:
    HardwareManager* hardwareManager;
    int currentScreen;
    bool led1StateLast;
    bool led2StateLast;
    bool switchStateLast;
    TFT_eSPI tft;
    void checkForChanges();
};

#endif // LCD_MANAGER_H
