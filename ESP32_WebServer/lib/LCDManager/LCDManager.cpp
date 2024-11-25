// LCDManager.cpp
#include "LCDManager.h"
#include "LCD_image.c" // Assuming this contains your image data

LCDManager::LCDManager(HardwareManager* hardwareManager)
    : hardwareManager(hardwareManager), currentScreen(0), tft(TFT_eSPI()) {}

void LCDManager::begin() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void LCDManager::update() {
    bool led1State = hardwareManager->getLED1State();
    bool led2State = hardwareManager->getLED2State();
    bool switchState = hardwareManager->isSwitchOn();
    if (led1State != led1StateLast || led2State != led2StateLast || switchState != switchStateLast) {
        // Only update the screen if there's a change
        tft.fillScreen(TFT_BLACK);  // Clear the screen
        if (currentScreen == 0) {
            displayLED1Status();
        } else {
            displayLED2Status();
        }
        // Update the last known states
        led1StateLast = led1State;
        led2StateLast = led2State;
        switchStateLast = switchState;
    }
}

void LCDManager::nextScreen() {
    currentScreen = (currentScreen + 1) % 2;  // Toggle between 0 and 1
    update();  // Update the display to show the current screen
}

void LCDManager::displayLED1Status() {
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString("LED1 Status", 10, 10);
    
    // Display LED1 indicator
    int led1State = hardwareManager->getLED1State();
    tft.fillCircle(64, 50, 15, led1State ? TFT_GREEN : TFT_DARKGREY);

    // Display Switch status
    tft.drawString("Switch:", 10, 90);
    if (hardwareManager->isSwitchOn()) {
        tft.pushImage(70, 90, 30, 15, LCD_ON);
    } else {
        tft.pushImage(70, 90, 30, 15, LCD_OFF);
    }
}

void LCDManager::displayLED2Status() {
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString("LED2 Status", 10, 10);
    
    // Display LED2 indicator
    int led2State = hardwareManager->getLED2State();
    tft.fillCircle(64, 50, 15, led2State ? TFT_RED : TFT_DARKGREY);

    // Display Switch status
    tft.drawString("Switch:", 10, 90);
    if (hardwareManager->isSwitchOn()) {
        tft.pushImage(70, 90, 30, 15, LCD_ON);
    } else {
        tft.pushImage(70, 90, 30, 15, LCD_OFF);
    }
}

void LCDManager::updateSwitchStatus() {
    // Update the display for switch status
    if (hardwareManager->isSwitchOn()) {
        tft.pushImage(70, 90, 30, 15, LCD_ON);
    } else {
        tft.pushImage(70, 90, 30, 15, LCD_OFF);
    }
}

