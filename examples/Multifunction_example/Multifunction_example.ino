#include <FingerPrintUDP.h>

const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;  // For feedback LED (can also use on-board LED)
const char* allowedUsername = "lokesh";

// Secure UDP Authenticator
FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);

// Sensor & actuator pins
const int ldrPin = 34;       // Analog pin for LDR
const int mq2Pin = 35;       // Analog pin for MQ2 Gas sensor
const int buzzerPin = 25;    // Buzzer pin
const int actionLed = 26;    // Custom LED pin

void securedMultiSensorAction() {
  Serial.println("Authenticated! Running secured multi-sensor function...");

  // 🔆 LDR Light level reading
  int ldrValue = analogRead(ldrPin);
  Serial.print("LDR (Light) Value: ");
  Serial.println(ldrValue);

  // 🌫️ MQ2 Gas sensor reading
  int mq2Value = analogRead(mq2Pin);
  Serial.print("MQ2 (Gas) Value: ");
  Serial.println(mq2Value);

  // 🔔 Buzzer Alert
  Serial.println("Buzzer ON...");
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);

  // 💡 LED Blinking
  for (int i = 0; i < 3; i++) {
    digitalWrite(actionLed, HIGH);
    delay(200);
    digitalWrite(actionLed, LOW);
    delay(200);
  }

  Serial.println("Multi-sensor action completed.\n");
}

void setup() {
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  pinMode(actionLed, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(mq2Pin, INPUT);

  authenticator.begin();
  authenticator.setAction(securedMultiSensorAction);
}

void loop() {
  authenticator.handleUDP();
}
