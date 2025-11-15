#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C tempLCD(0x25, 16, 2);  // Temperature display
LiquidCrystal_I2C humLCD(0x26, 16, 2);   // Humidity display
LiquidCrystal_I2C soilLCD(0x27, 16, 2);  // Soil moisture display

//Define sensor pins
int soilSensorPin = A0;  // Soil moisture sensor analog pin
int dht22SensorPin = 3;  // DHT22 sensor digital pin

//Define output pins
int soilRelayPin = 6;  //S Relay control digital pin
int humRelayPin = 11;  //H Relay control digital pin
int tempFanPin = 8;    //T Relay control digital pin

//Define sensor values and standerd values
int moistureValue = 0;       // Variable to store sensor value
int standardMoisture = 500;  // Adjust this depending on soil (0-1023)

double tempValue = 0;         // Variable to store sensor value
double standardTemp = 28.00;  // Adjust this depending on temp

double humValue = 0;        // Variable to store sensor value
double standardHum = 80.0;  // Adjust this depending on Hum

#define DHTTYPE DHT22
DHT dht(dht22SensorPin, DHTTYPE);

#define LCD_MOVERIGHT 0x04

void setup() {
  Serial.begin(9600);

  Wire.begin();  // Initialize I2C communication

  pinMode(soilSensorPin, INPUT);  //Soil moisture sensor pin as input
  pinMode(soilRelayPin, OUTPUT);  // Relay pin as output

  pinMode(dht22SensorPin, INPUT);  //DHT22 sensor pin as input
  pinMode(tempFanPin, OUTPUT);     // Relay pin as output
  pinMode(humRelayPin, OUTPUT);    // Relay pin as output

  digitalWrite(soilRelayPin, LOW);
  Serial.println("Soil Moisture Monitoring...");
  soilLCD.init();
  soilLCD.backlight();
  soilLCD.setCursor(0, 0);
  soilLCD.print("Soil Moisture");
  soilLCD.setCursor(0, 1);
  soilLCD.print("Monitoring...");
  delay(3000);
  soilLCD.clear();

  digitalWrite(humRelayPin, HIGH);
  Serial.println("Humidity Monitoring...");
  humLCD.init();
  humLCD.backlight();
  humLCD.setCursor(0, 0);
  humLCD.print("Humidity ");
  humLCD.setCursor(0, 1);
  humLCD.print("Monitoring...");
  delay(3000);
  humLCD.clear();

  digitalWrite(tempFanPin, HIGH);
  Serial.println("Temperature Monitoring...");
  tempLCD.init();
  tempLCD.backlight();
  tempLCD.setCursor(0, 0);
  tempLCD.print("Temperature");
  tempLCD.setCursor(0, 1);
  tempLCD.print("Monitoring...");
  delay(3000);
  tempLCD.clear();

  dht.begin();
}

void loop() {
  Serial.println("--------------------------------------------------------");
  // Read moisture level
  moistureValue = analogRead(soilSensorPin);

  //Check if soil moisture sensor readings are valid
  if (moistureValue == 1023 || moistureValue == 0) {
    Serial.println("Soil moisture sensor may be disconnected!");
    soilLCD.setCursor(0, 0);
    soilLCD.print("Soil moisture ");
    soilLCD.setCursor(0, 1);
    soilLCD.print("Sensor Error!      ");
    return;

  } else {

    //Print values
    Serial.print("Moisture:");
    Serial.println(moistureValue);

    soilLCD.setCursor(0, 0);
    soilLCD.print("Moisture:");
    soilLCD.setCursor(10, 0);
    soilLCD.print(moistureValue);
    soilLCD.setCursor(13, 0);
  }
  //---------------------------------------------------------

  // Check Temp and hum readings are valid

  tempValue = dht.readTemperature();
  humValue = dht.readHumidity();

  if (false) { 
    Serial.println("DHT22 sensor may be disconnected!");
    tempLCD.setCursor(0, 0);
    tempLCD.print("DHT22 Error!      ");
    delay(1000);

    humLCD.setCursor(0, 0);
    humLCD.print("DHT22 Error!      ");
    delay(1000);
    return;

  } else {
    // Print values
    Serial.print("Temperature: ");
    Serial.print(tempValue);
    Serial.println("°C");

    tempLCD.setCursor(0, 0);
    tempLCD.print("Temp:");
    tempLCD.setCursor(9, 0);
    tempLCD.print(tempValue);
    tempLCD.setCursor(15, 0);
    tempLCD.print("C");

    Serial.print("Humidity: ");
    Serial.print(humValue);
    Serial.println("%");

    humLCD.setCursor(0, 0);
    humLCD.print("Humidity:");
    humLCD.setCursor(10, 0);
    humLCD.print(humValue);
    humLCD.setCursor(15, 0);
    humLCD.println("%");

    Serial.println("---------------------------------------------------------");

    // Check soil condition
    if (moistureValue < standardMoisture) {

      // Soil is wet
      digitalWrite(soilRelayPin, HIGH);
      Serial.println("Soil is WET: Pump OFF");

      soilLCD.setCursor(0, 1);
      soilLCD.println("Soil is WET:OFF ");

    } else {
      // Soil is dry
      digitalWrite(soilRelayPin, LOW);  // Turn pump OFF
      Serial.println("Soil is DRY : Pump ON");

      soilLCD.setCursor(0, 1);
      soilLCD.println("Soil is DRY:ON  ");
    }

    Serial.println("---------------------------------------------------------");
    // Read temperature level
    tempValue = dht.readTemperature();

    // Check temperature condition
    if (tempValue < standardTemp) {
      // Temperature is Low
      digitalWrite(tempFanPin, HIGH);  // Turn fan OFF
      Serial.println("Temperature is LOW :Fan OFF");

      tempLCD.setCursor(0, 1);
      tempLCD.print("Temp LOW:Fan OFF");
    } else {

      //Temperature is High
      digitalWrite(tempFanPin, LOW);  // Turn fan ON
      Serial.println("Temperature is HIGH :Fan ON");
      tempLCD.setCursor(0, 1);
      tempLCD.print("Temp HIGH:Fan ON");
    }
    Serial.println("---------------------------------------------------------");
    // Read humidity level
    humValue = dht.readHumidity();

    // Check humidity condition
    if (humValue > standardHum) {
      // Humidty is high
      digitalWrite(humRelayPin, LOW);  // Turn pump OFF
      Serial.println("Humidity is HIGH :Pump OFF");

      humLCD.setCursor(0, 1);
      humLCD.print("Hum HIGH:Pump OFF");
    } else {
      // Humidity is low
      digitalWrite(humRelayPin, HIGH);  // Turn pump ON
      Serial.println("Humidity is LOW :Pump ON");

      humLCD.setCursor(0, 1);
      humLCD.print("Hum LOW:Pump ON");
    }
  }
}