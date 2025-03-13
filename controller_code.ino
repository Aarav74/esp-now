#include <ESP8266WiFi.h>
#include <espnow.h>

// Replace with the MAC address of the bot ESP8266
//C8:C9:A3:69:94:09

uint8_t botAddress[] = {0xC8, 0xC9, 0xA3, 0x69, 0x94, 0x09}; 

typedef struct {
  char command[20]; // Command to send to the bot
} Message;

Message data;

const int joystickX = A0;   // Joystick X-axis pin
const int pushButton1 = D6; // Forward button
const int pushButton2 = D7; // Backward button

void onSent(uint8_t *macAddr, uint8_t sendStatus) {
  Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(botAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_register_send_cb(onSent);

  // Set pin modes
  pinMode(joystickX, INPUT);
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
}

void loop() {
  // Default command
  strcpy(data.command, "STOP");

  // Read joystick X-axis for turning
  int xValue = analogRead(joystickX);
  if (xValue < 400) {
    strcpy(data.command, "TURN_LEFT");
  } else if (xValue > 600) {
    strcpy(data.command, "TURN_RIGHT");
  }

  // Read push buttons for forward/backward movement
  if (digitalRead(pushButton1) == LOW) {
    strcpy(data.command, "FORWARD");
  } else if (digitalRead(pushButton2) == LOW) {
    strcpy(data.command, "BACKWARD");
  }

  // Send command to the bot
  esp_now_send(botAddress, (uint8_t *)&data, sizeof(data));
  delay(100); // Small delay to avoid flooding
}
