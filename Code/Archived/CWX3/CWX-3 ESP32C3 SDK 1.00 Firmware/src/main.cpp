/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>

#include <Config.h> // User Configuration Variables.  Update filename as needed for a different board config.

#include <I2C.h>
#include <ADC.h>
#include <OLED.h>
#include <EEPROM_AT24.h>

#include <LED.h>
#include <OneWireSensor.h>
#include <BME280Sensor.h>
#include <SCD4xSensor.h>

#include <RebootMCU.h>

#include <WiFi-OTA.h>
#include <NTP.h>
#include <RTC.h>

#include <Domoticz.h>
#include <MQTT.h>
#include <ThingSpeakIoT.h>

// **************** FUNCTIONS AND ROUTINES ****************

// Read Sensors and Publish

void ReadSensors()
{
  // Basic Handler for WebServer
  if (WiFiEnabled == true)
  {
    CheckOTA();
  }

  // Update Time
  ReadNTP();
  ReadRTC();

  // Sensors
  ReadADC();
  ReadBME280Sensor();
  ReadSCD4xSensor();
  ReadOneWire();

  Serial.println("---------------------------------\n");

  StatusOLED();

  // Publish values to Domoticz
  if (WiFiEnabled == true)
  {
    PublishMQTTValues();
    PublishDomoticzValues();
    // PublishThingSpeakValues(myWriteAPIKey1, myChannelNumber1);
    PublishThingSpeakValues(myChannelNumber1);
  }
} // ReadSensors

// **************** SETUP ****************
void setup()
{
  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1); // 115200
  while (!Serial)
    ;
  Serial.println("");

  PrintUnderline("Hardware Firmware");

  // Heading
  Serial.println("DitroniX ESP32C3 Firmware Version: " + AppVersion);
  Serial.println("Location: \t" + LocationName);
  Serial.printf("Chip Model: \t%s\n", ESP.getChipModel());
  Serial.printf("Chip SN ID: \t%04X", (uint16_t)(chipid >> 32));
  Serial.printf("%08X\n", (uint32_t)chipid);
  Serial.println("\n");

  // Initialise Hardware
  PrintUnderline("Initialise");

  // Configure GPIO Inputs
  pinMode(User_Button, INPUT);
  pinMode(GPIO_VBAT, INPUT);
  pinMode(GPIO_LIGHT, INPUT);

  // Initialise I2C and Auto Enable Devices
  InitialiseI2C();
  ScanI2CBus();

  // Dynamic Headings
  AppName = AppAcronym + " ESP32-C3 Air Quality IoT and Weather Station Monitor";                               // Default Board Base Acronym - Do Not Change
  FirmwarePath = "[" + AppAcronym + " Firmware Folder Root] > pio > build > esp32-c3-devkitm-1 > firmware.bin"; // Default Board Base Acronym - Do Not Change

  // Initialise Sensors
  if (SCD4xEnabled == true)
    InitialiseSCD4xSensor();

  if (BME280Enabled == true)
    InitialiseBME280Sensor();

  // Initialise OneWire
  Initialise_OneWire();

  // Hardware Tests
  LED_Test();

  // Initialize OLED
  if (OLEDEnabled == true)
    InitialiseOLED();

  // Initialize EEPROM (Format if needed)
  if (EEPROMEnabled == true)
    InitialiseEEPROM();

  // Initialise WiFi OTA NTP
  InitialiseWiFi();
  InitialiseNTP();
  UpdateNTP();
  InitialiseWebServer();

  // Initialise Publishing to Domoticz MQTT ThingSpeak.  Comment out sections if needed.
  InitialiseDomoticz();
  InitialiseMQTT();
  InitialiseThingSpeak();
  Serial.println("");

  // Read Sensors and Publish
  ReadSensors();

  // Refresh OLED
  StatusOLED();

} // setup

// **************** LOOP ****************
void loop()
{
  // Check if Reboot Needed
  RebootCheck();

  // // Read Sensors and Publish
  ReadSensors();
  StatusOLED();

  // Basic Test for User Button Non-Interrupt Press and Hold
  if (digitalRead(User_Button) == LOW)
  {
    // Read Sensors and Publish
    ReadSensors();
    StatusOLED();
  }

  // Heatbeat LED
  if (WiFi.status() == WL_CONNECTED)
    led.flash(RGBLed::BLUE, 20);
  else
    led.flash(RGBLed::RED, 20);

  // Loop Delay
  delay(LoopDelay * 1000);
} // loop