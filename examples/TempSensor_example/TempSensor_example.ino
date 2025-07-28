#include <FingerPrintUDP.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;
const char* allowedUsername = "lokesh";

FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);
DHT dht(DHTPIN, DHTTYPE);

void readTemperature() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  authenticator.begin();
  authenticator.setAction(readTemperature);
}

void loop() {
  authenticator.handleUDP();
}
