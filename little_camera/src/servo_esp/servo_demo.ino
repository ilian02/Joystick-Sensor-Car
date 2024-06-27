
#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>

#define HORIZONTAL_SERVO_PIN 23 // ESP32 pin GPIO26 connected to servo motor
#define VERTICAL_SERVO_PIN 22 // ESP32 pin GPIO26 connected to servo motor
Servo horizontalServo;
Servo verticalServo;

int hServoDeg = 90;
int vServoDeg = 90;

typedef struct struct_message {
    char command[64];
    int value;
} struct_message;

void setup() {
  
  Serial.begin(115200);
  
  horizontalServo.attach(HORIZONTAL_SERVO_PIN, 400, 2400);  // attaches the servo on ESP32 
  verticalServo.attach(VERTICAL_SERVO_PIN, 400, 2400);  // attaches the servo on ESP32 pin

  horizontalServo.write(hServoDeg);
  verticalServo.write(vServoDeg);
  
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
  
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  struct_message myData;

  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Command: ");
  Serial.println(myData.command);

  if (myData.value == -1) {
    Serial.println(myData.command);
  }

  myData.value = 2;

  if (myData.command[0] == 'r') {
    if (hServoDeg + myData.value <= 140) {
      hServoDeg += myData.value;
    } else {
      hServoDeg = 140;
    }
    Serial.print("Horizontal tilt: ");
    Serial.println(hServoDeg);
    horizontalServo.write(hServoDeg);
  } else if (myData.command[0] == 'l') {
    if (hServoDeg - myData.value > 0) {
      hServoDeg -= myData.value;
    } else {
      hServoDeg = 0;
    }
    Serial.print("Horizontal tilt: ");
    Serial.println(hServoDeg);
    horizontalServo.write(hServoDeg);
  } else if (myData.command[0] == 'u') {
    if (vServoDeg + myData.value <= 140) {
      vServoDeg += myData.value;
    } else {
      vServoDeg = 140;
    }
    Serial.print("Vertical tilt: ");
    Serial.println(vServoDeg);
    verticalServo.write(vServoDeg);
  } else if (myData.command[0] == 'd') {
    if (vServoDeg - myData.value > 0) {
      vServoDeg -= myData.value;
    } else {
      hServoDeg = 0;
    }
    Serial.print("Vertical tilt: ");
    Serial.println(vServoDeg);
    verticalServo.write(vServoDeg);
  }
  
}

void loop() {

}