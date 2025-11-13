/*
 * Three LCD Displays - Simple Messages
 * LCD1 (0x25): "Hello World" + address
 * LCD2 (0x26): "Welcome" + address  
 * LCD3 (0x27): "Amoda" + address
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize three LCD displays with their addresses
LiquidCrystal_I2C lcd1(0x25, 16, 2);  // Hello World
LiquidCrystal_I2C lcd2(0x26, 16, 2);  // Welcome
LiquidCrystal_I2C lcd3(0x27, 16, 2);  // Amoda

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Initializing three LCD displays...");
  
  // Initialize LCD1 (0x25)
  lcd1.init();
  lcd1.backlight();
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Hello World");
  lcd1.setCursor(0, 1);
  lcd1.print("Address: 0x25");
  Serial.println("LCD1 (0x25): Hello World - Ready");
  
  // Initialize LCD2 (0x26)
  lcd2.init();
  lcd2.backlight();
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("Welcome");
  lcd2.setCursor(0, 1);
  lcd2.print("Address: 0x26");
  Serial.println("LCD2 (0x26): Welcome - Ready");
  
  // Initialize LCD3 (0x27)
  lcd3.init();
  lcd3.backlight();
  lcd3.clear();
  lcd3.setCursor(0, 0);
  lcd3.print("Amoda");
  lcd3.setCursor(0, 1);
  lcd3.print("Address: 0x27");
  Serial.println("LCD3 (0x27): Amoda - Ready");
  
  Serial.println("All displays initialized and messages displayed!");
}

void loop() {
  // Nothing in loop - displays will show static messages
  // Messages are displayed once in setup() and remain on screen
  
  // Optional: Add a heartbeat indicator every 10 seconds
  delay(10000);
  Serial.println("All displays running normally...");
}