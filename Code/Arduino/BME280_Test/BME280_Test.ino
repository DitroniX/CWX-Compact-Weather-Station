/*
  Dave Williams, DitroniX 2019-2026 (ditronix.net)
  CWX(C6), AQIX and AQIL Compact Weather Station and Air Quality Monitors - ESP32-C6 Series Board
  Features include ESP32-C6, BME280, SCD40/SCD41 Air Quality CO2 Sensor, SFH2440 Light Sensor, CR123A, Battery Sensor, USB ESP32C6 Dev Board  Flashing Compatible

  PCA 1.2603 - CWX(C6), AQIX and AQIL - Basic Bring Up Test Code - March 2026
 
  Remember!
  - Set the BOARD to Use ESP32C6 Dev Module (or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  - The Serial Monitor is configured for BAUD 115200
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website or github.com/DitroniX

  CWX           https://github.com/DitroniX/CWX-Compact-Weather-Station
  AQIX          https://github.com/DitroniX/AQIX-Air-Quality-Monitor-IoT-Weather-Station
  AQIL (LoRA)   https://github.com/DitroniX/AQIL-ESP32-C6-SX1262-LoRa-Air-Quality-Monitor-IoT-Weather-Station
*/

// Libraries
#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_BME280.h>  // Remember to install the Adafruit_BME280 library
#include <Adafruit_Sensor.h>

// ######### VARIABLES / DEFINES / STATIC #########

// Variables
char SensorResult[10];
float TemperatureC;
float Humidity;
int Pressure;

// Constants
const int LoopDelay = 1;  // Loop Delay in Seconds

// Objects
Adafruit_BME280 bme;  //BME280 connect to ESP32-C6 I2C BME280 0x76 | BMP280 0x76 |  BMP180 0x77

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// **************** OUTPUTS ****************
#define LED_Red 22    // Red LED
#define LED_Green 21  // Gren LED
#define LED_Blue 23   // Blue LED

// ######### FUNCTIONS #########

// Initialise BME280
void InitialiseTemperatureSensor() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!  Could eqally be a BMP180?");
    // BME280 0x76 | BMP280 0x76 |  BMP180 0x77
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

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // LEDs
  pinMode(LED_Red, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, LOW);

  InitialiseTemperatureSensor();
  Serial.println("");

  Serial.println("CWX(C6), AQIX and AQIL Configured...\n");
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

  Serial.println("\n");

  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);

  // Loop Delay
  delay(LoopDelay * 1000);
}
