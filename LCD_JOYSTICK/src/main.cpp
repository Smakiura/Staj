#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Initialize display instance

// Screen dimensions and joystick pins (as digital buttons)
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 128;
const int JOYSTICK_LEFT_PIN = 22;   // Joystick left direction
const int JOYSTICK_RIGHT_PIN = 21;  // Joystick right direction
const int JOYSTICK_UP_PIN = 4;      // Joystick up direction
const int JOYSTICK_DOWN_PIN = 25;    // Joystick down direction
const int BUTTON_PIN = 33;          // Joystick center button (KEY1)

// Circle properties
int circleX = SCREEN_WIDTH / 2;
int circleY = SCREEN_HEIGHT / 2;
int circleRadius = 10;

void setup() {
    tft.init();
    tft.setRotation(1); // Adjust rotation if necessary
    tft.fillScreen(TFT_BLACK);

    // Set joystick pins as inputs with pull-up resistors
    pinMode(JOYSTICK_LEFT_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_RIGHT_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_UP_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Pull-up for center button

    // Draw initial stationary circle
    tft.fillCircle(circleX, circleY, circleRadius, TFT_WHITE);
}

void loop() {
    // Erase previous circle only if a movement is detected
    bool moved = false;

    // Move circle based on joystick direction
    if (digitalRead(JOYSTICK_LEFT_PIN) == LOW) { // Left pressed
        tft.fillCircle(circleX, circleY, circleRadius, TFT_BLACK); // Erase previous circle
        circleX -= 2;
        moved = true;
    } 
    if (digitalRead(JOYSTICK_RIGHT_PIN) == LOW) { // Right pressed
        tft.fillCircle(circleX, circleY, circleRadius, TFT_BLACK); // Erase previous circle
        circleX += 2;
        moved = true;
    }
    if (digitalRead(JOYSTICK_UP_PIN) == LOW) { // Up pressed
        tft.fillCircle(circleX, circleY, circleRadius, TFT_BLACK); // Erase previous circle
        circleY -= 2;
        moved = true;
    }
    if (digitalRead(JOYSTICK_DOWN_PIN) == LOW) { // Down pressed
        tft.fillCircle(circleX, circleY, circleRadius, TFT_BLACK); // Erase previous circle
        circleY += 2;
        moved = true;
    }

    // Check boundaries
    circleX = constrain(circleX, circleRadius, SCREEN_WIDTH - circleRadius);
    circleY = constrain(circleY, circleRadius, SCREEN_HEIGHT - circleRadius);

    // Draw circle only if it moved
    if (moved) {
        tft.fillCircle(circleX, circleY, circleRadius, TFT_WHITE);
    }

    // Reset position if button is pressed
    if (digitalRead(BUTTON_PIN) == LOW) {
        tft.fillCircle(circleX, circleY, circleRadius, TFT_BLACK); // Erase previous position
        circleX = SCREEN_WIDTH / 2;
        circleY = SCREEN_HEIGHT / 2;
        tft.fillCircle(circleX, circleY, circleRadius, TFT_WHITE); // Draw circle at center
    }

    delay(30); // Control loop speed
}
