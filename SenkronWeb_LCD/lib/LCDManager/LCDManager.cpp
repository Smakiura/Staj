#include "LCDManager.h"
#include "LCD_image.c"
#include <Arduino.h>

LCDManager::LCDManager(HardwareManager* hardwareManager)
    : hardwareManager(hardwareManager), currentScreen(0), lastUpdateTime(0), joystickDelay(200), lastJoystickUpdateTime(0) {}

void LCDManager::begin() {
    tft.init();
    tft.setRotation(1);  // Adjust based on your screen orientation
    tft.fillScreen(TFT_BLACK);  // Set background color to black

    // Initial state of LEDs and switch
    lastLED1State = hardwareManager->getLED1State();
    lastLED2State = hardwareManager->getLED2State();
    lastSwitchState = hardwareManager->isSwitchOn();
}

void LCDManager::update() {
    unsigned long currentMillis = millis();
    
    // Joystick delay to prevent flickering when changing screens
    if (currentMillis - lastJoystickUpdateTime >= joystickDelay) {
        lastJoystickUpdateTime = currentMillis;

        // Left/Right to navigate between screens
        if (hardwareManager->isJoystickLeftPressed()) {
            if (currentScreen == 0) {
                currentScreen++;  
                drawScreen();
            }
            else if (currentScreen == 1) {
                currentScreen--;  
                drawScreen();
            }
        } else if (hardwareManager->isJoystickRightPressed()) {
            if (currentScreen == 1) {
                currentScreen--;  
                drawScreen();
            }
            else if (currentScreen == 0) {
                currentScreen++; 
                drawScreen();
            }
        }

        // Up/Down to toggle LEDs
        if (hardwareManager->isJoystickUpPressed()) {
            if (currentScreen == 0) {
                hardwareManager->turnLED1On();  // Turn LED1 ON
            } else if (currentScreen == 1) {
                hardwareManager->turnLED2On();  // Turn LED2 ON
            }
            drawScreen();
        } else if (hardwareManager->isJoystickDownPressed()) {
            if (currentScreen == 0) {
                hardwareManager->turnLED1Off();  // Turn LED1 OFF
            } else if (currentScreen == 1) {
                hardwareManager->turnLED2Off();  // Turn LED2 OFF
            }
            drawScreen();
        }
        
    }
      // Check for any state change in LEDs or switch to update LCD
    bool currentLED1State = hardwareManager->getLED1State();
    bool currentLED2State = hardwareManager->getLED2State();
    bool currentSwitchState = hardwareManager->isSwitchOn();

    // Update LCD if there's a change in any LED or switch state
    if (currentLED1State != lastLED1State || currentLED2State != lastLED2State || currentSwitchState != lastSwitchState) {
        drawScreen();  // Update the LCD screen
        lastLED1State = currentLED1State;
        lastLED2State = currentLED2State;
        lastSwitchState = currentSwitchState;
    }
    
 
}

void LCDManager::setJoystickState(bool joystickLeft, bool joystickRight, bool joystickUp, bool joystickDown) {
    
}

void LCDManager::drawScreen() {
    tft.fillScreen(TFT_BLACK); // Clear the screen
    if (currentScreen == 0) {
        drawLED1Status();  // Draw the first LED screen
    } else {
        drawLED2Status();  // Draw the second LED screen
    }
}

void LCDManager::drawLED1Status() {
    tft.setCursor(10, 10);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE); // White text color for contrast on black background
    tft.print("GREEN LED: ");
    int led1State = hardwareManager->getLED1State();
    tft.fillCircle(64, 50, 15, led1State ? TFT_GREEN : TFT_DARKGREY);

   
}

void LCDManager::drawLED2Status() {
    tft.setCursor(10, 10);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE); // White text color for contrast on black background
    tft.print("RED LED: ");
    int led2State = hardwareManager->getLED2State();
    tft.fillCircle(64, 50, 15, led2State ? TFT_RED : TFT_DARKGREY);

    tft.setTextSize(1);
     tft.drawString("Switch:", 10, 90);
    if (hardwareManager->isSwitchOn()) {
        tft.pushImage(70, 90, 30, 15, LCD_ON);
    } else {
        tft.pushImage(70, 90, 30, 15, LCD_OFF);
    }
}
