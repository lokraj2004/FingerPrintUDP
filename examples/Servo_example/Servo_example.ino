#include <FingerPrintUDP.h>
#include <ESP32Servo.h>

const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;
const char* allowedUsername = "lokesh";

FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);

Servo myServo;

void moveServo() {
  for (int pos = 0; pos <= 180; pos += 10) {
    myServo.write(pos);
    delay(200);
  }
  for (int pos = 180; pos >= 0; pos -= 10) {
    myServo.write(pos);
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  myServo.attach(13); // Replace with your pin
  authenticator.begin();
  authenticator.setAction(moveServo);
}

void loop() {
  authenticator.handleUDP();
}
