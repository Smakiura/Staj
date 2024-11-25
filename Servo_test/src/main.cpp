#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <SPIFFS.h>

// Servo and Ultrasonic Sensor setup
Servo myServo;
const int servoPin = 15;
const int trigPin = 18;
const int echoPin = 19;

// WiFi and Server setup
const char* ssid = "sensor-wifi";
const char* password = "sensorlab2017";
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Variables for sensor and servo control
bool isSweeping = false;
unsigned long lastSweepTime = 0;
const int sweepDelay = 100;  // delay between each sweep position
int angle = 0;

// Function to calculate distance using the ultrasonic sensor
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 17647);
  return duration * 0.034 / 2;
}

// WebSocket event handler to send sensor data to clients
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                      void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("Client connected!");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Client disconnected");
  }
}

// Setup WiFi, WebSocket, and sensor
void setup() {
  Serial.begin(9600);

  if (!SPIFFS.begin(true)) {  // true enables formatting if the file system is not found
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted successfully");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize the servo
  myServo.attach(servoPin);

  // Set up sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize WebSocket and set up event handler
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Start server
  server.begin();
  Serial.println("Server started");
}

// Perform sweeping and send data to WebSocket clients
void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastSweepTime >= sweepDelay) {
    lastSweepTime = currentMillis;
    String distances = "";

    // Sweep the servo from 0 to 180 degrees
    for (int angle = 0; angle <= 180; angle += 10) {
      myServo.write(angle);
      delay(100);  // Wait for the servo to position
      long distance = getDistance();
      distances += String(angle) + "," + String(distance) + ";";  // Send angle and distance

      // Send the data for each scan
      ws.textAll(distances);

      delay(500);
    
    }
  }

  // Handle WebSocket events
  ws.cleanupClients();
}
