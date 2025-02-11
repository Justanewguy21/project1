#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

// Replace with your Blynk Auth Token
char auth[] = "YourAuthToken";

const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 16;

// Blynk virtual pins for LED control
BLYNK_WRITE(V1) {
  digitalWrite(ledPin1, param.asInt()); 
}
BLYNK_WRITE(V2) {
  digitalWrite(ledPin2, param.asInt());
}
BLYNK_WRITE(V3) {
  digitalWrite(ledPin3, param.asInt());
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth);
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  Blynk.run(); 
}
