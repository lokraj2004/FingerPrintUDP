#include <FingerPrintUDP.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;
const char* allowedUsername = "lokesh";

FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void showMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("Hello Lokesh!");
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  authenticator.begin();
  authenticator.setAction(showMessage);
}

void loop() {
  authenticator.handleUDP();
}
