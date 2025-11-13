#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorPin = A0;    // Soil moisture sensor analog pin
int relayPin = 2;      // Relay control digital pin
int moistureValue; // Variable to store sensor value
int threshold = 500;   // Adjust this depending on soil (0-1023)

#define LCD_MOVERIGHT 0x04

void setup() {
 pinMode(A0, INPUT);
 pinMode(10, OUTPUT);
 Serial.begin(9600);
 lcd.init();
 lcd.backlight();
 delay(1000);
 lcd.clear();
 delay(1000);
 lcd.setCursor(0, 0);
 lcd.print("Good Morning ");
 Serial.println("LCD Printing...");
 delay(1000);

 pinMode(sensorPin , INPUT); //sensor pin as input
 pinMode(relayPin, OUTPUT);   // Relay pin as output          
 digitalWrite(relayPin, LOW); // Ensure pump is OFF at start
 lcd.setCursor(0, 0);
 lcd.print("System Ready: Soil Moisture Monitoring...");
 Serial.println("System Ready: Soil Moisture Monitoring...");
 delay(1000);

}
void loop() {
 // Read moisture level
  moistureValue = analogRead(sensorPin);

  // Print value to Serial Monitor
  Serial.print("Moisture Value: ");
  lcd.setCursor(0, 1);
  lcd.print("MoistureValue: ");
  Serial.println(moistureValue);
  lcd.setCursor(13, 1);
  lcd.print(moistureValue);

  // Check soil condition
  if (moistureValue > threshold) {
    // Soil is dry
    digitalWrite(relayPin, HIGH); // Turn pump ON
    Serial.println("Soil is DRY : Pump ON");
    lcd.setCursor(0, 0);
    lcd.print("Soil is DRY : Pump ON");
    
  } 
  else {
    // Soil is wet
    digitalWrite(relayPin, LOW);  // Turn pump OFF
    Serial.println("Soil is WET:Pump OFF");
    lcd.setCursor(0, 0);
    lcd.print("SoilisWET:PumpOFF");
  }

  delay(1000); // 1 second delay before next reading
}

