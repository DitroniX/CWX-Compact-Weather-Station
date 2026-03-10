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

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// **************** FUNCTIONS AND ROUTINES ****************

// I2C Bus Scanner
void ScanI2CBus() {

  // Useful Reference https://i2cdevices.org/addresses

  byte error, address;
  int nDevices;

  Serial.println("Scanning I2C Bus for Devices for Auto Configuration ...\n");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    nDevices++;
    if (nDevices == 1)
      Serial.println(" I2C Possible Device(s) found at Address:");
    if (error == 0) {
      Serial.print(" * Hex 0x");
      Serial.print(address, HEX);

      Serial.print(" (");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(")\t");

      switch (address) {
        case 0x3C ... 0x3D:
          Serial.print(" OLED");
          break;
        case 0x50:
          Serial.print(" EEPROM");
          break;
        case 0x58:
          Serial.print(" EEPROM (Reserved Address)");
          break;
        case 0x62:
          Serial.print(" SCD40 or SCD41 CO2 Sensor");
          break;
        case 0x76:
          Serial.print(" BME280 Temp Humidity Pressure Sensor");
          break;
        default:
          Serial.print(" ?");
          break;
      }
      Serial.println();
    } else if (error == 4) {
      Serial.print(" * Unknown error at address Decimal ");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println(" * No I2C devices found. Possible Hardware Issue?");
  } else {
    Serial.println(" I2C Bus Scan Complete\n");
  }
}  // ScanI2CBus

// **************** SETUP ****************
void setup() {

  // Stabalise (Optional)
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1);  // 115200
  while (!Serial)
    ;
  Serial.println("");

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("CWX(C6), AQIX and AQIL Configured...\n");

  ScanI2CBus();
}

// **************** LOOP ****************
void loop() {
}
