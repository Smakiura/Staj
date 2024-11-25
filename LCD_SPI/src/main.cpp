#include <TFT_eSPI.h>      // Include the TFT_eSPI library
#include <stdint.h>  // This will define uint16_t
#include "Resim.c"       // Include your image data file (generated from Image2CPP or another tool)

// Initialize TFT display
TFT_eSPI tft = TFT_eSPI();  

void setup() {
    tft.begin();                    // Initialize the TFT screen
    tft.setRotation(1);              // Set screen orientation (adjust if needed)
    tft.fillScreen(TFT_BLACK);       // Clear the screen with black color

    // Display the image on the TFT screen at position (0, 0)
    tft.pushImage(0, 0, 128, 128, myImage);
}

void loop() {
    // Nothing to do here, the image is displayed once on setup
    // If you want to update or animate the image, you can add code here
}
