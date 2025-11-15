#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Pin Definitions ===
#define DHTPIN A0
#define RELAY_PIN 3
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if needed

// === Thresholds ===
#define TEMP_ON 25.0f
#define TEMP_OFF 20.0f
#define HUM_ON 60.0f
#define HUM_OFF 70.0f

bool pumpState = false; // Track pump state

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF (assuming active LOW)
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Check for sensor failure
  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT sensor error!");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C, Hum: ");
  Serial.print(hum);
  Serial.println(" %");

  // === Control Logic ===
  if (!pumpState && (temp >= TEMP_ON || hum <= HUM_ON)) {
    pumpState = true;
    digitalWrite(RELAY_PIN, LOW); // Turn ON pump (active LOW)
  }
  else if (pumpState && (temp <= TEMP_OFF && hum >= HUM_OFF)) {
    pumpState = false;
    digitalWrite(RELAY_PIN, HIGH); // Turn OFF pump
  }

  delay(1000);
}