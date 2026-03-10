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
#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2cScd4x.h>

// ****************  VARIABLES / DEFINES / STATIC ****************

SensirionI2cScd4x scd4x;

// Constants
const int LoopDelay = 1;  // Loop Delay in Seconds

// Variables
static char errorMessage[64];
static int16_t error;

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

// **************** OUTPUTS ****************
#define LED_Red 22    // Red LED
#define LED_Green 21  // Gren LED
#define LED_Blue 23   // Blue LED

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// ######### FUNCTIONS #########

void PrintUint64(uint64_t& value) {
  Serial.print("0x");
  Serial.print((uint32_t)(value >> 32), HEX);
  Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}

// ######### SETUP #########
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART:
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

  Serial.println("");

  uint16_t error;
  char errorMessage[256];

  scd4x.begin(Wire, SCD41_I2C_ADDR_62);

  Serial.println("Checking Sensor");

  uint64_t serialNumber = 0;
  delay(30);
  // Ensure scd4x is in clean state
  error = scd4x.wakeUp();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute wakeUp(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  error = scd4x.stopPeriodicMeasurement();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  error = scd4x.reinit();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute reinit(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  // Read out information about the scd4x
  error = scd4x.getSerialNumber(serialNumber);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  Serial.print("SCD4x Sensor Serial Number:\t 0x");
  PrintUint64(serialNumber);
  Serial.println("\n");
  //
  // If temperature offset and/or scd4x altitude compensation
  // is required, you should call the respective functions here.
  // Check out the header file for the function definitions.

  Serial.println("CWX(C6), AQIX and AQIL Configured...\n");
}

// ######### LOOP #########
void loop() {

  Serial.println("Preparing to Read Sensor");

  uint16_t co2Concentration = 0;
  float temperature = 0.0;
  float relativeHumidity = 0.0;

  // Wake the scd4x up from sleep mode.
  error = scd4x.wakeUp();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute wakeUp(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  // Ignore first measurement after wake up.
  error = scd4x.measureSingleShot();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute measureSingleShot(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  // Perform single shot measurement and read data.
  error = scd4x.measureAndReadSingleShot(co2Concentration, temperature, relativeHumidity);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute measureAndReadSingleShot(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  // Display Readings
  Serial.print("CO2 concentration [ppm]:\t ");
  Serial.println(co2Concentration);

  Serial.print("Temperature [°C]:\t\t ");
  Serial.println(temperature);

  Serial.print("Relative Humidity [RH]:\t\t ");
  Serial.println(relativeHumidity);

  Serial.println("\n");

  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(150);
  digitalWrite(LED_Red, LOW);

  // Sleep
  Serial.print("Sleep for 15 seconds until next measurement is due");
  Serial.println();
  delay(15000);  
}
