 /*   
Developer Note – N. Lokeshraj
I am N. Lokeshraj, a final-year Electronics and Communication Engineering student at Meenakshi Sundararajan Engineering College. I have developed the FingerPrintUDP.h header file to support app-specific UDP-based authentication in the FingerPrint UDP Sensor Android application.

This header enables ESP32 microcontrollers to securely interact with the mobile app using the smartphone’s built-in fingerprint sensor. Both the ESP32 and the mobile device must be connected to the same network—either a personal hotspot or a Wi-Fi router—to enable communication.

Example setup: Turn on the hotspot on device X, then connect the ESP32 and the app-installed mobile device Y to it. Once connected, authenticated control actions can be triggered securely from the mobile device to the ESP32.

 
 */




#include <FingerPrintUDP.h>

const char* ssid = "your ssid";
const char* password = "your password";
const int udpPort = 4210; //default udpport recommended
const int ledPin = 2; // default ledbuilt-in pin in esp32 variants. But some variants may change
const char* allowedUsername = "lokesh"; // you can provide your username here

FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername); // authenticator class initialization

void SecuredFunction() {
  Serial.println("✅ You are authenticated!");
  delay(2000);
  Serial.println("✅ Task completed: Box Unlocked!");
}

void setup() {
  Serial.begin(115200);
  authenticator.begin();
  authenticator.setAction(SecuredFunction); // Function which is handled with security
}

void loop() {
  authenticator.handleUDP(); // Function called, so that it is performed continuously.
}