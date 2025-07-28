#include <FingerPrintUDP.h>
#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// --- Fingerprint Auth Config ---
const char* ssid = "yourSSID";
const char* password = "yourPassword";
const int udpPort = 4210;
const int ledPin = 2;
const char* allowedUsername = "lokesh";

FingerPrintUDP authenticator(ssid, password, udpPort, ledPin, allowedUsername);

// --- DHT Sensor ---
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- Soil Moisture ---
const int soilPin = 34;

// --- Servo for Door Lock (Secured) ---
Servo doorServo;
const int servoPin = 26;

// --- OLED Display ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Secured Function ---
void unlockDoor() {
  Serial.println("🔐 Authenticated. Unlocking door...");
  doorServo.write(90);  // Unlock
  delay(3000);
  doorServo.write(0);   // Lock again
  Serial.println("🔒 Door relocked.");
}

// --- Unsecured: Display Sensors Info (Always Visible) ---
void updateSensorDisplay() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int soil = analogRead(soilPin);

  // Serial Monitor
  Serial.print("🌡️ Temp: "); Serial.print(temp); Serial.print("°C | ");
  Serial.print("💧 Hum: "); Serial.print(hum); Serial.print("% | ");
  Serial.print("🌱 Soil: "); Serial.println(soil);

  // OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Greenhouse Monitor");
  display.print("Temp: "); display.print(temp); display.println(" C");
  display.print("Humidity: "); display.print(hum); display.println(" %");
  display.print("Soil: "); display.println(soil);
  display.display();
}

void setup() {
  Serial.begin(115200);

  // Init peripherals
  dht.begin();
  pinMode(soilPin, INPUT);
  doorServo.attach(servoPin);
  doorServo.write(0); // Locked

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ OLED not found!");
    while (true);
  }

  display.clearDisplay();
  display.display();

  // Authenticator
  authenticator.begin();
  authenticator.setAction(unlockDoor);
}

void loop() {
  authenticator.handleUDP();    // 🔐 Wait for secured trigger
  updateSensorDisplay();        // ⚙️ Unsecured actions run always
  delay(2000);
}
