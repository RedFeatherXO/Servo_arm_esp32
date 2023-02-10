#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x40, 0x22, 0xD8, 0xFF, 0xBE, 0x6C};

int RightStick_X_pin = 32;
int LeftStick_X_pin = 33;
int RightStick_Y_pin = 34;
int LeftStick_Y_pin = 35;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int LeftStick_X;
  int LeftStick_Y;
  int RightStick_X;
  int RightStick_Y;
} struct_message;

// Create a struct_message called myData
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

  pinMode(RightStick_X_pin,INPUT);
  pinMode(LeftStick_X_pin,INPUT);
  pinMode(RightStick_Y_pin,INPUT);
  pinMode(LeftStick_Y_pin,INPUT);
 
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
  // Set values to send
  myData.LeftStick_X = analogRead(LeftStick_X_pin);
  myData.LeftStick_Y = analogRead(LeftStick_Y_pin);
  myData.RightStick_X = analogRead(RightStick_X_pin);
  myData.RightStick_Y = analogRead(RightStick_Y_pin);
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10);
}
