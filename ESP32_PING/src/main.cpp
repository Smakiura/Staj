#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>  // Install this library via Library Manager (Tools > Manage Libraries)
#define LED_BUILTIN 2

// Replace these with your Wi-Fi network credentials
const char* ssid = "sensor-wifi";
const char* password = "sensorlab2017";

// Remote host to ping
IPAddress remote_ip(8, 8, 8, 8);  // Google DNS server

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to Wi-Fi network
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void led_ping(int LED, int rep) {
  for(int i=rep; i>0; i--) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}

void loop() {
  // Ping the remote host
  if (Ping.ping(remote_ip)) {
    Serial.println("Ping successful!");
    led_ping(LED_BUILTIN, 2);
  }
  else {
    Serial.println("Ping failed!");
    led_ping(LED_BUILTIN, 3);
  }

  delay(5000);  // Wait 5 seconds before the next ping
}