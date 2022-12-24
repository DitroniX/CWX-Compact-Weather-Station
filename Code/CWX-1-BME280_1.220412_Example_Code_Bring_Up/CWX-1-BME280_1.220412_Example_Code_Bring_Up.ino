/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX 12S Compact Weather Station ESP8266 SDK
  Features include ESP8266, CR123A, BME280, SFH2440 Light Sensor, UART Flashing Compatible
  
  PCA 1.2204-106 - CWX-1_1.220406_Basic_Bring_Up_Test_Code_MQTT Firmware 1.220406 - 6th April 2022
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
 
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX

  Example Output:
  Temperature (ºC):  22.2
  Humidity (%):    49
  Pressure (hPa): 988
  Light: 62
  System Uptime 0 days, 0 hours, 0 minutes, 14 seconds  
*/

// Libraries
#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "uptime_formatter.h"  // https://github.com/YiannisBourkelis/Uptime-Library

// ######### OBJECTS #########
Adafruit_BME280 bme;  //BME280 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)  // BME280 0x76 | BMP280 0x76 |  BMP180 0x77

// ######### VARIABLES / DEFINES / STATIC #########

// App
String AppVersion = "v1.220412";
String AppBuild = "DitroniX CWX-1-BME280 12S SDK PCA v2204-106";
String AppName = "Compact Weather Station - Basic Bring Up Test";

// Variables
char SensorResult[10];
float TemperatureC;
float Humidity;
int Pressure;
int LightMeter;

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5

// **************** OUTPUTS ****************
#define LED_Module 2  // Define ESP Output Port LED

// **************** INPUTS ****************
#define ADC A0  // Define ADC (0 DC - 1V DC)

// ######### FUNCTIONS #########

// Scan for I2C Devices
void scan_i2c_devices() {
  // Scan I2C Devices
  Serial.print("Scanning I2C\t");
  byte count = 0;
  Serial.print("Found Devices: ");
  Serial.print(" Devices: ");
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.print(")\t");
      count++;
      delay(1);
    }
  }
  Serial.print("Found ");
  Serial.print(count, HEX);
  Serial.println(" Device(s).");
}

// Initialise BME280
void InitialiseTemperatureSensor() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    // while (1);
  }
}

// ######### SETUP #########
void setup() {

  //Stabalise
  delay(500);

  // Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial)
    ;
  Serial.println("");
  Serial.println(AppVersion + " Initialized");
  Serial.println(AppBuild);
  Serial.println(AppName);
  Serial.println("");

  // LED
  pinMode(LED_Module, OUTPUT);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  scan_i2c_devices();
  InitialiseTemperatureSensor();

  Serial.println("");
}

// ######### LOOP #########
void loop() {

  // Get Temperature ºC and round to 1 decimal place
  TemperatureC = bme.readTemperature();
  dtostrf(TemperatureC, 5, 1, SensorResult);
  sprintf(SensorResult, "%s", SensorResult);
  Serial.print("Temperature (ºC): ");
  Serial.println(SensorResult);

  // Get Humity % and round to no decimal places
  Humidity = round(bme.readHumidity());
  dtostrf(Humidity, 5, 0, SensorResult);
  Serial.print("Humidity (%): ");
  Serial.println(SensorResult);

  // Get Pressure and round to no decimal places
  Pressure = (bme.readPressure() / 100.0F);
  Pressure = round(Pressure);
  Serial.print("Pressure (hPa): ");
  Serial.println(Pressure);

  // Get Light Reading
  LightMeter = analogRead(ADC);
  Serial.print("Light: ");
  Serial.print(LightMeter);  // data output from the photoresistor (0-1024)
  Serial.println("");

  // Timestamp
  Serial.println("System Uptime " + uptime_formatter::getUptime());
  Serial.println("");

  // Hearbeat LED (Temporary during development)
  digitalWrite(LED_Module, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_Module, HIGH);  // turn the LED off by making the voltage LOW

  // Loop Delay
  delay(1000);
}
