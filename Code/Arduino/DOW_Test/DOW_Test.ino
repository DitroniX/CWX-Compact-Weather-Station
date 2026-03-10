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
#include <OneWire.h>
#include <DallasTemperature.h>

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// **************** OUTPUTS ****************
#define LED_Red 22    // Red LED
#define LED_Green 21  // Gren LED
#define LED_Blue 23   // Blue LED

// OneWire
#define ONE_WIRE_BUS 13


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ######### SETUP #########
void setup(void) {
  // start serial port
  Serial.begin(115200);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // LEDs
  pinMode(LED_Red, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, LOW);

  Serial.println("CWX(C6), AQIX and AQIL Configured...\n");
}

// ######### LOOP #########
void loop(void) {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();  // Send the command to get temperatures
  Serial.println("DONE\n");
  delay(1500);

  // After we received the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } else {
    Serial.println("Error: Could not read temperature data\n");
  }


  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);

  // Loop Delay
  delay(LoopDelay * 1000);  
}