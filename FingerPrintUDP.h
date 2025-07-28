#ifndef FINGERPRINT_UDP_H
#define FINGERPRINT_UDP_H

#include <WiFi.h>
#include <WiFiUdp.h>

class FingerPrintUDP {
public:
  FingerPrintUDP(const char* ssid, const char* password, int udpPort, int ledPin, const char* allowedUsername);
  void begin();
  void handleUDP(); // Call this in loop()
  void setAction(void (*task)()); // Set the task to perform on successful authentication

private:
  const char* ssid;
  const char* password;
  int udpPort;
  int ledPin;
  const char* allowedUsername;

  WiFiUDP udp;
  IPAddress remoteIP;
  int remotePort;
  char incomingPacket[255];

  String currentToken;
  unsigned long lastConnectTime;
  const unsigned long timeoutDuration = 15000; // 15 seconds
  bool tokenActive;

  void (*authenticatedTask)(); // Function pointer to user-defined task

  String generateToken();
  void blinkConnectPattern();
  void blinkUnlockPattern();
  void sendUDP(const String& message);
};

#endif
