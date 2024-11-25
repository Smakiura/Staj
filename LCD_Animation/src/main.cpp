#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Initialize display instance

// Screen dimensions
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 128;

// Circle properties
int circleX = random(10, SCREEN_WIDTH - 10);
int circleY = random(10, SCREEN_HEIGHT - 10);
int circleRadius = 10;

// Movement speed and direction
int xSpeed = 3;
int ySpeed = 2;

// Color variable and function to change it
uint16_t currentColor = TFT_WHITE;

void changeColor() {
    currentColor = tft.color565(random(255), random(255), random(255));
}

void setup() {
    tft.init();
    tft.setRotation(1); // Adjust if necessary
    tft.fillScreen(TFT_BLACK);
}

void loop() {
    // Erase previous circle position
    tft.fillCircle(circleX, circleY, circleRadius, TFT_BLACK);

    // Update circle position
    circleX += xSpeed;
    circleY += ySpeed;

    // Check for collision with the screen edges
    if (circleX <= circleRadius || circleX >= SCREEN_WIDTH - circleRadius) {
        xSpeed = -xSpeed; // Reverse x direction
        changeColor();    // Change color on bounce
    }
    if (circleY <= circleRadius || circleY >= SCREEN_HEIGHT - circleRadius) {
        ySpeed = -ySpeed; // Reverse y direction
        changeColor();    // Change color on bounce
    }

    // Draw circle at new position
    tft.fillCircle(circleX, circleY, circleRadius, currentColor);

    delay(30); // Control animation speed
}

