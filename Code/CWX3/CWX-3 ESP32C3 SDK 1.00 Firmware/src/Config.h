/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  This config file can be duplicated for multiple boards, such as config1.h, config2.h etc..  Simply changing the include filename in main.cpp at complile time.
*/

// Libraries
#include <Arduino.h>

// ****************  GLOBAL VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ******************************** Main ********************************

// User Constants
int LoopDelay = 5; // Loop Delay in Seconds (Default 5)
int OLEDDelay = 2; // OLED Delay in Seconds (Default 2)

// Application Variables
String AppAcronym = "CWX3";                                                                                          // Default Board Base Acronym - Do Not Change
String AppName = AppAcronym + " ESP32-C3 IoT Weather Station Monitor";                               // Default Board Base Acronym - Do Not Change
String FirmwarePath = "[" + AppAcronym + " Firmware Folder Root] > pio > build > esp32-c3-devkitm-1 > firmware.bin"; // Default Board Base Acronym - Do Not Change

// Application Constants
const String AppVersion = "241012"; // Internal Firmware Date Code
const String LocationName = "Home"; // Enter Name Location of Device. Used for Serial Monitor and OLED.

// ******************************** ADC ********************************

// Variables User
const int AverageSamples = 20; // Average Multi-Samples on each Channel Read.  (Default 20)
const int AverageDelay = 20;   // Average Inter Multi-Sample ms Delay.  (Default 20)

// Voltage User
const float DCVoltageThreshold = 6.00; // Minimum Voltage > USB - removing noise. (Default 6.00)
const float DCVoltageOffset = 0.00;    // Battery Voltage Zero Calibration Offset.  (Default 0.00)

// Calibration
float ADC_Calibration = 3.300; // Adjust as needed for final calibration of ADC_Voltage.
float BAT_Calibration = 3.000; // Adjust as needed for final calibration of Lithium Battery Voltage.

// Sensor Externs - No Change
uint16_t BME280_Pressure;        // BME280 Pressure
uint16_t BME280_Humidity;        // BME280 Humidity
float BME280_Temperature = 0.0f; // BME280 Temperature

uint16_t SCD4x_CO2;                   // SCD4x CO2 (AQIX Only)
uint16_t SCD4x_eCO2;                  // SCD4x eCO2 - Estimated concentration level of CO2 (AQIX Only) [WIP]
float SCD4x_Humidity;                 // SCD4x Humidity (AQIX Only)
float SCD4x_Temperature = 0.0f;       // SCD4x Temperature (AQIX Only)
float SCD4x_TemperatureOffset = 0.0f; // SCD4x Temperature Offset (AQIX Only)
float SCD4x_DewPoint;                 // SCD4x Humidity (AQIX Only)
uint16_t SCD4x_Altitude;              // SCD4x Altitude in Metres (AQIX Only)

uint16_t SFH_LightRaw;    // SFH2440 Light Raw Value
float SFH_LightDC = 0.0f; // SFH2440 Light DC Value
int SFH_LightPercentage;  // SFH2440 Light Percentage Value

float Batt_Voltage = 0.0f; // Battery 3V3 Voltage
int Batt_Percentage;       // Battery 3V3 Percentage

float DS_TemperatureC[9]; // Dallas OneWire Temperatures C (Up to 10 Sensors)
float DS_TemperatureF[9]; // Dallas OneWire Temperatures F (Up to 10 Sensors)

// ******************************** WiFi ********************************

// WiFi.  Setup with your Wireless Information.  Add more as needed.
const char *ssid1 = "";     // WiFi Network SSID - Case Sensitive
const char *password1 = ""; // WiFi Network password - Case Sensitive
const char *ssid2 = "";     // WiFi Network SSID - Case Sensitive
const char *password2 = ""; // WiFi Network password - Case Sensitive
const char *ssid3 = "";     // WiFi Network SSID - Case Sensitive
const char *password3 = ""; // WiFi Network password - Case Sensitive

// WiFi. Force Disable for Testing.  !!!!! BEWARE Outside of Local Developmet Do NOT Disable as Network OTA updating will NOT work !!!!!
const boolean WiFiEnabled = false; // Enable/Disable WiFi for Local USB Development and Testing Only.  You can leave SSID/IP etc populated.

// Set your static or fixed IP address and gateway - Alternatively leave at (0, 0, 0, 0) for standard DHCP
IPAddress local_IP(0, 0, 0, 0);     // Leave at (0, 0, 0, 0) if DHCP required.  (Default 0, 0, 0, 0)
IPAddress gateway(0, 0, 0, 0);      // Set to your Router IP = Gateway.  (Default 0, 0, 0, 0)
IPAddress subnet(255, 255, 255, 0); // Should not need to change subnet from default (255, 255, 255, 0)

// Optional DNS Defaults.
IPAddress primaryDNS(0, 0, 0, 0);   // Defaults to your above Gateway IP if left as (0, 0, 0, 0)
IPAddress secondaryDNS(8, 8, 4, 4); // For Google Public DNS use for Primary or Secondary (8,8,8,8) and/or (8,8,4,4)

// WiFi Other User Options
int WifiMaxAPScan = 5;                   // Multiple WiFi Scan Maximum APs (Default 5)
const uint32_t connectTimeoutMs = 10000; // WiFi connect timeout per AP. Increase when connecting takes longer. (Default 10000)

// WebServer User Options
boolean EnableAutoWebRefresh = true; // Change to false, to stop WebServer Page Auto Refresh.  (Default true)

// NTP Time Server
boolean EnableAutoNTPRefresh = true; // Change to false, to stop NTP Auto Refresh.  (Default true)
const char *ntpServer = "europe.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

// Externs - No Change
String HostName;    // Hostname
int RSSI_Level;     // Wifi RSSI Signal Level
String RSSI_Detail; // Wifi RSSI Signal Information
String MACAddress;  // MAC Address
String WiFi_SSID;   // WiFi SSID

// ******************************** MQTT ********************************

// Enable MQTT Publishing
boolean EnableMQTT = false; // Change to true, to enable Loop reading and sending data to MQTT.  (Default false)

// MQTT Server info.  Setup with your MQTT Configration
const char *MQTTServer = "0.0.0.0";       // MQTT Server IP Address (Typically a Fixed Local Address).  (Default 0.0.0.0)
int MQTTPort = 1883;                      // MQTT Network Port (Default 1883)
const char *MQTTUser = "";                // MQTT User - if applicable
const char *MQTTPassword = "";            // MQTT Password - if applicable.  Also see !secret mqtt_password
const String MQTTTopicMain = "";          // MQTT Topic Header.  Change main topic as needed such as domoticz/out
const int MQTTQoS = 0;                    // MQTT Quality of Service (QoS). QoS 0, commonly called 'fire and forget' (Default 0)
const boolean MQTTRetain = true;          // MQTT state messages should be retained. (Default true)
const boolean MQTTForceTopicLower = true; // MQTT Force Topic to be all lowecase. (Default true)
String MQTTTopic;                         // MQTT Topic. (Auto Configured)

// Callback
boolean EnableMQTTCallBack = false; // Under Development. (Default false)

// Send these values to MQTT Broker.  Set to true to publish value, or set to false and entry is ignored.
const boolean mqttBME280_Humidity = true;    // BME280 Humidity
const boolean mqttBME280_Temperature = true; // BME280 Temperature
const boolean mqttBME280_Pressure = true;    // BME280 Pressure

const boolean mqttSCD4x_CO2 = false;                // SCD4x CO2 (AQIX Only) - PublishDomoticzNumeric
const boolean mqttSCD4x_eCO2 = false;              // SCD4x eCO2 (AQIX Only) [WIP] - PublishDomoticzNumeric
const boolean mqttSCD4x_Humidity = false;           // SCD4x Humidity (AQIX Only) - PublishDomoticzNumeric
const boolean mqttSCD4x_Temperature = false;        // SCD4x Temperature (AQIX Only)
const boolean mqttSCD4x_TemperatureOffset = false; // SCD4x Temperature Offset (AQIX Only)
const boolean mqttSCD4x_DewPoint = false;           // SCD4x Calculated Dewpoint (AQIX Only)
const boolean mqttSCD4x_Altitude = false;          // SCD4x Altitude in Metres (AQIX Only)

const boolean mqttSFH_LightRaw = true;        // SFH2440 Light Raw Value
const boolean mqttSFH_LightDC = true;         // SFH2440 Light DC Value
const boolean mqttSFH_LightPercentage = true; // SFH2440 Light Percentage Value

const boolean mqttBatt_Voltage = true;    // Battery 3V3 Voltage
const boolean mqttBatt_Percentage = true; // Battery 3V3 Percentage

// ******************************** Domoticz ********************************

// Enable Publishing
const boolean EnableDomoticz = true; // Change to true, to enable Loop reading and sending data to Domoticz.  (Default false)

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char *DomoticzServer = "0.0.0.0"; // Domoticz Server IP Address (Typically a Fixed Local Address)  (Default 0.0.0.0)
const int DomoticzPort = 8080;          // Domoticz Network Port (Default 8080)
const char *Domoticz_User = "";         // Domoticz User - if applicable  (Not currently implimented in the connection)
const char *Domoticz_Password = "";     // Domoticz Password - if applicable  (Not currently implimented in the connection)

// Domoticz Hardware Device Indexes

// Set these values to the Domoticz Devices Indexes (IDX).  If Zero, then entry is ignored.
// Remember a device first needs to be created in Domoticz, then use related IDX number here.   i.e.  129 = Domoticz hardware index 129.

const int idxBME280_Humidity = 0;    // BME280 Humidity
const int idxBME280_Temperature = 0; // BME280 Temperature
const int idxBME280_Pressure = 0;    // BME280 Pressure

const int idxSCD4x_CO2 = 0;               // SCD4x CO2 (AQIX Only) - PublishDomoticzNumeric
const int idxSCD4x_eCO2 = 0;              // SCD4x eCO2 (AQIX Only) [WIP] - PublishDomoticzNumeric
const int idxSCD4x_Humidity = 0;          // SCD4x Humidity (AQIX Only) - PublishDomoticzNumeric
const int idxSCD4x_Temperature = 0;       // SCD4x Temperature (AQIX Only)
const int idxSCD4x_TemperatureOffset = 0; // SCD4x Temperature Offset (AQIX Only)
const int idxSCD4x_DewPoint = 0;          // SCD4x Calculated Dewpoint (AQIX Only)
const int idxSCD4x_Altitude = 0;          // SCD4x Altitude in Metres (AQIX Only)

const int idxSFH_LightRaw = 0;        // SFH2440 Light Raw Value
const int idxSFH_LightDC = 0;         // SFH2440 Light DC Value
const int idxSFH_LightPercentage = 0; // SFH2440 Light Percentage Value

const int idxBatt_Voltage = 0;    // Battery 3V3 Voltage
const int idxBatt_Percentage = 0; // Battery 3V3 Percentage

// Set this value to the Domoticz Device Group Index (IDX) - Note: Currently Unused Virtual Device.
const int idxDomoticzBase = 0;        // If Zero, then entry is ignored.  Group device needs to be created in Domoticz. WIP.
const int idxDomoticzOneWireBase = 0; // If Zero, then entry is ignored.  Group device needs to be created in Domoticz. WIP.
const int DomoticzOneWireMax = 0;     // Set Maximum Dynamic Sensors from idxDomoticzOneWireBase

// ******************************** ThingSpeak ********************************

// Enable Publishing
boolean EnableThingSpeak = false; // Change to true, to enable Loop reading and sending data to ThingSpeak.  (Default false).

// ThingSpeak Server info.  Setup with your ThingSpeak Configuration
// Typically a single channel and Write API Key would be required.  Example PublishThingSpeakValues(myWriteAPIKey1, myChannelNumber1);
// To publish to ThingSpeak, you need a Write API Key, Channel ID and Field Number.
const char *myWriteAPIKey1 = ""; // ThingSpeak Write API Key for Channel ID 1
const unsigned long myChannelNumber1 = 0;  // ThingSpeak Channel ID 1 (Default 0)
const char *yWriteAPIKey2 = "";                  // ThingSpeak Write API Key for Channel ID 2
const unsigned long myChannelNumber2 = 0;        // ThingSpeak Channel ID 2 (Default 0)
const char *myWriteAPIKey3 = "";                 // ThingSpeak Write API Key for Channel ID 3
const unsigned long myChannelNumber3 = 0;        // ThingSpeak Channel ID 3 (Default 0)

// ******************************** ReBoot ********************************

// 0 reset disabled
// 300000UL call reset every 300 seconds (5 Minutes)
// 3600000UL call reset every 60 mins (1 Hour)
// 86400000UL call reset every 24 hours (1 Day)
// 604800000UL call reset every 7 days (1 Week).
// 2592000000UL call reset every 30 days (1 Month)
time_t RebootTime = 604800000UL; // Configure as needed. (Default 604800000UL)

// ******************************** NTP ********************************

// Externs - No Change
String NTP_RTC, RTC_Time;
time_t timenow;

// ******************************** I2C ********************************

// Externs - No Change
boolean EEPROMEnabled = false; // Auto Configuration Use - Do  Not Change
boolean OLEDEnabled = false;   // Auto Configuration Use - Do  Not Change
boolean BME280Enabled = false; // Auto Configuration Use - Do  Not Change
boolean SCD4xEnabled = false;  // Auto Configuration Use - Do  Not Change
boolean NTPRefreshed = false;  // Auto Configuration Use - Do  Not Change

// Externs - No Change
int SensorCount;        // OneWire Sensor Count
boolean OneWireEnabled; // OneWire Auto Config

// ******************************** DATA ********************************

// Define I2C (Expansion Port)
#define I2C_SDA 7
#define I2C_SCL 5
#define I2C_CLOCK 4000000UL

// Define OneWire (Expansion Port)
#define DOW 6 // Dallas OneWire

// Default Addresses
#define BME280_Address 0x76
#define EEPROM_Address 0x50
#define SCD4x_Address 0x62 // (AQIX Only)

// ******************************** GPIO INPUTS ********************************
#define User_Button 9 // User Button also Manual PGM
#define GPIO_VBAT 3   // Voltage Sensor
#define GPIO_LIGHT 4  // Light Sensor

// ******************************** GPIO OUTPUTS ********************************
#define LED_Red 10   // User RGB
#define LED_Green 18 // User RGB
#define LED_Blue 19  // User RGB

// ******************************** RTC ********************************

// # RTC Clock Config WIP
// #
// # CONFIG_RTC_CLK_SRC_INT_RC is not set
// # CONFIG_RTC_CLK_SRC_EXT_CRYS is not set
// CONFIG_RTC_CLK_SRC_EXT_OSC=y
// # CONFIG_RTC_CLK_SRC_INT_8MD256 is not set
// CONFIG_RTC_CLK_CAL_CYCLES=5000
// # end of RTC Clock Config