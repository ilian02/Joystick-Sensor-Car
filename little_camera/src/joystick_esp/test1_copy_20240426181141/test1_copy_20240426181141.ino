#include <esp_now.h>
#include <WiFi.h>

const int RIGHT_PIN = 23;
const int LEFT_PIN = 22;
const int UP_PIN = 25;
const int DOWN_PIN = 26;

const int HORIZONTAL_VELOCITY = 5;
const int VERTICAL_VELOCITY = 8;


uint8_t broadcastAddress[] = {0x10, 0x97, 0xBD, 0xE4, 0x39, 0x50};

typedef struct struct_message {
  char command[64];
  int value;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {

  //int buttonState = digitalRead(DOWN_PIN);
  //Serial.println(buttonState);
  //delay(200);

  int stateUp = digitalRead(UP_PIN);
  int stateDown= digitalRead(DOWN_PIN);
  int stateLeft = digitalRead(LEFT_PIN);
  int stateRight = digitalRead(RIGHT_PIN);

  esp_err_t result;

  if (stateUp == 0) {
    strcpy(myData.command, "up");
    myData.value = HORIZONTAL_VELOCITY;
    result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));  
  }

  if (stateDown == 0) {
    strcpy(myData.command, "down");
    myData.value = HORIZONTAL_VELOCITY;
    result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));  
  }

  if (stateLeft == 0) {
    strcpy(myData.command, "left");
    myData.value = VERTICAL_VELOCITY;
    result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));  
  }

  if (stateRight == 0) {
    strcpy(myData.command, "right");
    myData.value = VERTICAL_VELOCITY;
    result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));  
  }
  
  /*
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  */
  delay(50);
}
