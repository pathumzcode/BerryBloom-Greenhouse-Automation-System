#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C tempLCD(0x25, 16, 2);    // Temperature display
LiquidCrystal_I2C humLCD(0x27, 16, 2); // Humidity display  
LiquidCrystal_I2C soilLCD(0x26, 16, 2);     // Soil moisture display

//Define sensor pins
int soilSensorPin = A1;    // Soil moisture sensor analog pin
int dht22SensorPin = 3; // DHT22 sensor digital pin

//Define output pins
int soilRelayPin = 6; // Relay control digital pin
int humRelayPin = 11; // Relay control digital pin
int tempFanPin = 8; // Fan control digital pin

//Define sensor values and standerd values
int moistureValue = 0; // Variable to store sensor value
int realSoilMoistureValue =0; //human-readable percentage of soil moisture after mapping.
int standardMoisture = 500;   // Adjust this depending on soil (0-1023)

double tempValue = 0; // Variable to store sensor value
double standardTemp = 31.1; // Adjust this depending on temp 

double humValue = 0; // Variable to store sensor value
double standardHum = 90.0; // Adjust this depending on Hum

#define DHTTYPE DHT22
DHT dht(dht22SensorPin,DHTTYPE);

#define LCD_MOVERIGHT 0x04

void setup() {
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();
  
  // Initialize all three LCD displays
  tempLCD.init();
  tempLCD.backlight();

  humLCD.init();
  humLCD.backlight();
  
  soilLCD.init();
  soilLCD.backlight();
  
  pinMode(soilSensorPin , INPUT); //Soil moisture sensor pin as input
  pinMode(soilRelayPin, OUTPUT);   // Relay pin as output

  pinMode(dht22SensorPin, INPUT); //DHT22 sensor pin as input
  pinMode(tempFanPin, OUTPUT);   // Relay pin as output 
  pinMode(humRelayPin, OUTPUT);   // Relay pin as output 

  dht.begin(); //Initialize DHT sensor
  
  digitalWrite(soilRelayPin,HIGH); // Ensure pump is OFF at start
  Serial.println("Soil Moisture Monitoring...");
  soilLCD.setCursor(0, 0);
  soilLCD.print("Soil Moisture");
  soilLCD.setCursor(0, 1);
  soilLCD.print("Monitoring...");
  delay(3000);
        
  digitalWrite(humRelayPin,HIGH);
  Serial.println("Humidity Monitoring...");
  humLCD.setCursor(0, 0);
  humLCD.print("Humidity ");
  humLCD.setCursor(0, 1);
  humLCD.print("Monitoring...");
  delay(3000);
  
  digitalWrite(tempFanPin,HIGH);
  Serial.println("Temperature Monitoring...");
  tempLCD.setCursor(0, 0);
  tempLCD.print("Temperature");
  tempLCD.setCursor(0, 1);
  tempLCD.print("Monitoring...");
  delay(3000);

  soilLCD.clear();
  humLCD.clear();
  tempLCD.clear();

} 

void loop() {
   Serial.println("--------------------------------------------------------");
     // Read moisture level
     moistureValue = analogRead(soilSensorPin);

     /*realSoilMoistureValue = map(moistureValue, 0, 30000, 0, 100);
     moistureValue = constrain(moistureValue, 0, 100);*/

     //Check if soil moisture sensor readings are valid
    if (moistureValue == 1023 || moistureValue == 0) {
     Serial.println("Soil moisture sensor may be disconnected!");

     soilLCD.setCursor(0, 0);
     soilLCD.print("Soil moisture ");
     soilLCD.setCursor(0, 1);
     soilLCD.print("Sensor Error!      ");
    }  else {
    
     //Print values
     Serial.print("Moisture:");
     Serial.println(moistureValue);
     //Serial.println("%");

     soilLCD.setCursor(0,0);
     soilLCD.print("Moisture:");
     soilLCD.setCursor(10,0);
     soilLCD.print(moistureValue);
     soilLCD.setCursor(13,0);
     //soilLCD.print("%");
    }
    //---------------------------------------------------------
     // Check Temp and hum readings are valid
   
   if (isnan(tempValue) || isnan(humValue)) {

     Serial.println("DHT22 sensor may be disconnected!");

     tempLCD.setCursor(0, 0);
     tempLCD.print("DHT22 Error!      ");
     delay(1000);

     humLCD.setCursor(0, 0);
     humLCD.print("DHT22 Error!      ");
     delay(1000);

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

  delay(1000); // 1 second delay before next reading
  Serial.println("---------------------------------------------------------");
     // Check soil condition
    if (moistureValue < standardMoisture) {
     // Soil is dry
     digitalWrite(soilRelayPin, HIGH); // Turn pump ON
     Serial.println("Soil is WET: Pump OFF");

     soilLCD.setCursor(0,1);
     soilLCD.println("Soil is WET:OFF ");
    
    }else {
     // Soil is wet
     digitalWrite(soilRelayPin, LOW);  // Turn pump OFF
     Serial.println("Soil is DRY : Pump ON");

     soilLCD.setCursor(0,1);
     soilLCD.println("Soil is DRY:ON  ");
    }
  delay(1000); // 1 second delay before next reading
  Serial.println("---------------------------------------------------------");
     // Read temperature level
     tempValue = dht.readTemperature(); // was digitalRead

     // Check temperature condition
    if (tempValue > standardTemp) {
     // Temperature is high
     digitalWrite(tempFanPin, HIGH); // Turn fan ON
     Serial.println("Temperature is HIGH :Fan ON");

     tempLCD.setCursor(0,1);
     tempLCD.print("Temp HIGH:Fan ON");
    } else {

     //Temperature is low
     digitalWrite(tempFanPin, LOW);  // Turn fan OFF
     Serial.println("Temperature is LOW :Fan OFF");

     tempLCD.setCursor(0,1);
     tempLCD.print("Temp LOW:Fan OFF");
    }
  delay(1000); // 1 second delay before next reading
  Serial.println("---------------------------------------------------------");
     // Read humidity level
     humValue = dht.readHumidity();

     // Check humidity condition
     if (humValue > standardHum) {
     // Humidty is high
     digitalWrite(humRelayPin, LOW);  // Turn pump OFF
     Serial.println("Humidity is HIGH :Pump OFF");

     humLCD.setCursor(0,1);
     humLCD.print("Hum HIGH:Pump OFF");
    } else {
     // Humidity is low
     digitalWrite(humRelayPin, HIGH); // Turn pump ON
     Serial.println("Humidity is LOW :Pump ON");

     humLCD.setCursor(0,1);
     humLCD.print("Hum LOW:Pump ON");
    }
  delay(1000); // 1 second delay before next reading
}
}