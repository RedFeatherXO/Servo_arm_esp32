#include <esp_now.h>
#include <WiFi.h>
#include <Servo.h>

Servo BaseServo;
Servo LowerServo;
Servo UpperServo;
int BaseServo_pin = 14;
int LowerServo_pin = 13;
int UpperServo_pin = 12;
int Basepos = 90;
int Lowerpos = 90;
int Upperpos = 90;
void MoveServo(Servo&,int&,int,int,int,int,char);

int offset = 15;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int LeftStick_X;
  int LeftStick_Y;
  int RightStick_X;
  int RightStick_Y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("READ DATA");
  MoveServo(BaseServo,Basepos,myData.RightStick_Y,7,0,180,'B');
  MoveServo(LowerServo,Lowerpos,myData.LeftStick_X,7,40,170,'L');
  MoveServo(UpperServo,Upperpos,myData.RightStick_X,7,180,90,'U');
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  BaseServo.attach(BaseServo_pin);
  LowerServo.attach(LowerServo_pin);
  UpperServo.attach(UpperServo_pin);
  // Set device as a Wi-Fi Station
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
 
void loop() {

}

void MoveServo(Servo &myServo,int &Start_pos,int target_pos,int _delay,int min_angle,int max_angle,char name){
  double start = Start_pos;
  //Serial.println(name);
  //Serial.print("RAW: ");
  //Serial.println(target_pos);
  target_pos = map(target_pos,0,4096,min_angle,max_angle);
  if(start<target_pos-offset) {
    for (start; start < target_pos-1; start += 1) {//180 <= 77
      //Serial.println("FOR - IF");
      myServo.write(start);
      delay(_delay);
      Start_pos = start;
    }
  } else if(start>target_pos+offset) { //180
    for (start; start > target_pos-1; start -= 1) {
      //Serial.println("ELSE - IF");
      myServo.write(start);
      delay(_delay);
      Start_pos = start;
    }
  }
}


  /*Serial.print("X-Links:");
  Serial.print(myData.LeftStick_X);
  Serial.print(",");
  Serial.print("Y-Links:");
  Serial.print(myData.LeftStick_Y);
  Serial.print(",");
  Serial.print("X-Rechts:");
  Serial.print(myData.RightStick_X);
  Serial.print(",");
  Serial.print("Y-Rechts:");
  Serial.println(myData.RightStick_Y);
  Serial.print("Lowerpos");
  Serial.println(Lowerpos);*/
