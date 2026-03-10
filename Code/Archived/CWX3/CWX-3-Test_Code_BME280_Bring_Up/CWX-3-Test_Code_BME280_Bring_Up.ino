/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)
  AQIX and CWX-3 Compact Weather Station ESP32-C3 Series Board
  Features include ESP32-C3, CR123A, BME280, SFH2440 Light Sensor, SCD40, USB D1 Flashing Compatible, Battery Sensor

  PCA 1.2409.101 - Compact Weather Station - Basic Bring Up Test Code - 28th Sepember 2024

  Simplified Board Bring Up and Test the BME-280
  
  Remember!
  - Set the BOARD to Use ESP32C3 Dev Module (or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  - The Serial Monitor is configured for BAUD 115200
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website or github.com/DitroniX

  Example Output:
  Temperature (ºC):  22.1
  Humidity (%):    46
  Pressure (hPa): 1013
*/

#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_BME280.h>  // Remember to install the Adafruit_BME280 library
#include <Adafruit_Sensor.h>

// ######### OBJECTS #########
Adafruit_BME280 bme;  //BME280 connect to ESP32-C3 I2C BME280 0x76 | BMP280 0x76 |  BMP180 0x77

// ######### VARIABLES / DEFINES / STATIC #########

// Variables
char SensorResult[10];
float TemperatureC;
float Humidity;
int Pressure;

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 7
#define I2C_SCL 5

// **************** OUTPUTS ****************
#define LED_Red 10 // Red LED
#define LED_Green 18 // Gren LED
#define LED_Blue 19 // Blue LED

// **************** INPUTS ****************
#define ADC A0  // Define ADC (0 DC - 1V DC)

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
  Wire.begin (I2C_SDA, I2C_SCL);

  // LEDs
  pinMode(LED_Red, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, LOW);

  Serial.println("AQIX or CWX3 Configured...");

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

    Serial.println("\n");

  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);

  // Loop Delay
  delay(LoopDelay * 1000);
}
