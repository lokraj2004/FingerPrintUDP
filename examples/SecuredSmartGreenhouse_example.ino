#include <FingerPrintUDP.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// --- Fingerprint UDP Setup ---
const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;
const char* allowedUsername = "lokesh";
FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);

// --- DHT Sensor Setup ---
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- OLED Setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Other Components ---
const int soilMoisturePin = 34;   // Analog pin
const int pumpRelayPin = 25;      // Relay for water pump
const int fanRelayPin = 26;       // Relay for fan
const int dryThreshold = 300;     // Soil dryness threshold
const float tempThreshold = 30.0; // Temp in °C

void securedGreenhouseTasks() {
  Serial.println("✅ Authenticated! Starting Smart Greenhouse Tasks...");

  // 🌱 Read Soil Moisture
  int soilValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);

  if (soilValue < dryThreshold) {
    Serial.println("💧 Soil is dry. Turning ON Water Pump...");
    digitalWrite(pumpRelayPin, HIGH);
    delay(2000);  // Water for 2 seconds
    digitalWrite(pumpRelayPin, LOW);
  } else {
    Serial.println("✅ Soil moisture is sufficient.");
  }

  // 🌡️ Read Temp & Humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C, ");
  Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");

  if (temp > tempThreshold) {
    Serial.println("🔥 High temp! Turning ON Fan...");
    digitalWrite(fanRelayPin, HIGH);
    delay(3000);
    digitalWrite(fanRelayPin, LOW);
  } else {
    Serial.println("✅ Temp is normal.");
  }

  // 🖥️ OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Smart Greenhouse");
  display.print("Soil: "); display.println(soilValue);
  display.print("Temp: "); display.print(temp); display.println(" C");
  display.print("Humidity: "); display.print(hum); display.println(" %");
  display.display();

  Serial.println("🌿 Task Done. Greenhouse updated.\n");
}

void setup() {
  Serial.begin(115200);

  // Initialize peripherals
  pinMode(soilMoisturePin, INPUT);
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW);
  digitalWrite(fanRelayPin, LOW);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }
  display.clearDisplay();
  display.display();

  // Begin UDP Auth
  authenticator.begin();
  authenticator.setAction(securedGreenhouseTasks);
}

void loop() {
  authenticator.handleUDP();
}
