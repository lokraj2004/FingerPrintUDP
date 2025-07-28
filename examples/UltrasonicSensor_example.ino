#include <FingerPrintUDP.h>

const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;
const char* allowedUsername = "lokesh";

FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);

const int trigPin = 5;
const int echoPin = 18;

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  authenticator.begin();
  authenticator.setAction(measureDistance);
}

void loop() {
  authenticator.handleUDP();
}
