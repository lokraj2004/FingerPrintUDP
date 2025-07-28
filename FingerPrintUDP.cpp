#include "FingerPrintUDP.h"

FingerPrintUDP::FingerPrintUDP(const char* ssid, const char* password, int udpPort, int ledPin, const char* allowedUsername)
  : ssid(ssid), password(password), udpPort(udpPort), ledPin(ledPin), allowedUsername(allowedUsername), tokenActive(false), authenticatedTask(nullptr) {}

void FingerPrintUDP::begin() {
  // Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  udp.begin(udpPort);
  Serial.printf("UDP server listening on port %d\n", udpPort);
}

void FingerPrintUDP::setAction(void (*task)()) {
  authenticatedTask = task;
}

void FingerPrintUDP::handleUDP() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = '\0';

    String received = String(incomingPacket);
    remoteIP = udp.remoteIP();
    remotePort = udp.remotePort();
    Serial.printf("Received from %s:%d -> %s\n", remoteIP.toString().c_str(), remotePort, received.c_str());

    if (received.startsWith("CONNECT:")) {
      String username = received.substring(8);
      if (username == allowedUsername) {
        currentToken = generateToken();
        lastConnectTime = millis();
        tokenActive = true;
        blinkConnectPattern();
        sendUDP("ACK:CONNECT:" + currentToken);
        Serial.println("Token issued: " + currentToken);
      } else {
        sendUDP("INVALID_USER");
        Serial.println("Invalid username received.");
      }
    }

    else if (received.startsWith("UNLOCK:") && tokenActive) {
      String token = received.substring(7);
      if (token == currentToken && (millis() - lastConnectTime) < timeoutDuration) {
         sendUDP("ACK:UNLOCK");
         blinkUnlockPattern();
        if (authenticatedTask) authenticatedTask();
        tokenActive = false;
      }
    }
  }

  // Token timeout
  if (tokenActive && (millis() - lastConnectTime) >= timeoutDuration) {
    Serial.println("Token expired. Sending TIMEOUT.");
    sendUDP("TIMEOUT");
    tokenActive = false;
  }
}

String FingerPrintUDP::generateToken() {
  String token = "";
  for (int i = 0; i < 6; i++) {
    token += String(random(0, 10));
  }
  return token;
}

void FingerPrintUDP::blinkConnectPattern() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH); delay(500);
    digitalWrite(ledPin, LOW); delay(100);
  }
}

void FingerPrintUDP::blinkUnlockPattern() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin, HIGH); delay(500);
    digitalWrite(ledPin, LOW); delay(100);
    digitalWrite(ledPin, HIGH); delay(500);
    digitalWrite(ledPin, LOW); delay(100);
  }
}

void FingerPrintUDP::sendUDP(const String& message) {
  udp.beginPacket(remoteIP, remotePort);
  udp.write((const uint8_t *)message.c_str(), message.length());
  udp.endPacket();
}
