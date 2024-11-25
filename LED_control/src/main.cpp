#include <Arduino.h>
 
#define LED_PIN 5           // PWM-capable pin for LED
#define SWITCH_PIN 13       // Button input pin

int brightness = 0;          // Current brightness (0-255)
int fadeAmount = 25;         // How much to change the brightness
bool lastButtonState = HIGH; // Previous button state
unsigned long lastDebounceTime = 0;  // Debounce timestamp
const unsigned long debounceDelay = 50;  // Debounce delay in ms

void setup() {
  pinMode(LED_PIN, OUTPUT);           // LED as output
  pinMode(SWITCH_PIN, INPUT_PULLUP);  // Switch with internal pull-up

  // PWM Setup: Channel 0, 5kHz frequency, 8-bit resolution
  ledcAttachPin(LED_PIN, 0);  
  ledcSetup(0, 5000, 8);  

  // Set initial LED brightness to 0
  ledcWrite(0, brightness);  
}

void loop() {
  int buttonState = digitalRead(SWITCH_PIN);  // Read button state

  // Debounce handling: Only act if state changes and delay has passed
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();  // Update debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState == LOW) {  // On valid button press
      brightness += fadeAmount;  // Change brightness

      // Reverse fade direction if brightness out of range
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }

      // Set new brightness using PWM
      ledcWrite(0, brightness);
      Serial.println(brightness);
    }
  }

  lastButtonState = buttonState;  // Update button state
  delay(200);  // Short delay to smooth loop
}
