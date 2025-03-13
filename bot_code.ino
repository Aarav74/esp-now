#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure for receiving data
typedef struct {
  char command[20];
} Message;

Message data;

// Motor control pins
const int motor1A = D1; // Left motor
const int motor1B = D2; // Left motor
const int motor2A = D3; // Right motor
const int motor2B = D4; // Right motor

// Callback function for received ESP-NOW data
void onReceive(uint8_t *macAddr, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Received Command: ");
  Serial.println(data.command);

  // Motor control logic
  if (strcmp(data.command, "FORWARD") == 0) {
    moveForward();
  } else if (strcmp(data.command, "BACKWARD") == 0) {
    moveBackward();
  } else if (strcmp(data.command, "TURN_LEFT") == 0) {
    turnLeft();
  } else if (strcmp(data.command, "TURN_RIGHT") == 0) {
    turnRight();
  } else if (strcmp(data.command, "STOP") == 0) {
    stopMotors();
  }
}

// Move forward
void moveForward() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  Serial.println("Moving forward");
}

// Move backward
void moveBackward() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  Serial.println("Moving backward");
}

// Turn left
void turnLeft() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  Serial.println("Turning left");
}

// Turn right
void turnRight() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  Serial.println("Turning right");
}

// Stop all motors
void stopMotors() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
  Serial.println("Stopping motors");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onReceive);

  // Initialize motor control pins
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  // Stop motors initially
  stopMotors();

  Serial.println("ESP-NOW Bot Initialized");
}

void loop() {
  // Wait for commands; ESP-NOW handles everything in the background
}
