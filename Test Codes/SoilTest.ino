#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorPin = A0;    // Soil moisture sensor analog pin
int relayPin = 2;      // Relay control digital pin
int moistureValue; // Variable to store sensor value
int threshold = 500;   // Adjust this depending on soil (0-1023)

void setup() {
  pinMode(sensorPin , INPUT); //sensor pin as input
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);   // Relay pin as output          
  digitalWrite(relayPin, LOW); // Ensure pump is OFF at start
  Serial.begin(9600);
  Serial.println("System Ready: Soil Moisture Monitoring...");
}

void loop() {
  // Read moisture level
  moistureValue = analogRead(sensorPin);

  // Print value to Serial Monitor
  Serial.print("Moisture Value: ");
  Serial.println(moistureValue);

  // Check soil condition
  if (moistureValue > threshold) {
    // Soil is dry
    digitalWrite(relayPin, HIGH); // Turn pump ON
    Serial.println("Soil is DRY : Pump ON");
    
  } 
  else {
    // Soil is wet
    digitalWrite(relayPin, LOW);  // Turn pump OFF
    Serial.println("Soil is WET : Pump OFF");
  }

  delay(1000); // 1 second delay before next reading
}
