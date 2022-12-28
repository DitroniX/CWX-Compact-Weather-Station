/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX-2 Compact Weather Station ESP8266 Series Board V2 (221228)
  Features include ESP8266, CR123A, BME280, BMP180, SFH2440 Light Sensor, USB D1 Flashing Compatible, Battery Sensor

  PCA 2212-202 - Compact Weather Station - Basic Bring Up Test Code (BMP085/BMP180)

  Simplified Board Bring Up and Test the BMP180 and SFH2440 Light Sensor.  Matrix Test. Battery Voltage Test.  No Deep-Sleep Enabled. Send to Domoticz
  
  Remember!
  - Set the BOARD to ESP8266 DEV Module (Any Wemos D1 or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX

  Example Output from USB 5V:
  Temperature (ºC):  22.5
  Pressure (hPa): 992
  Sending THB Message to Domoticz #30
  Light: 379
  ADC Value 	881	ADC Voltage 3.27 V Battery 166 % 	 (Probably DC Powered)
*/

#include "Arduino.h"
#include "Wire.h"
#include <ESP8266WiFi.h>
#include <Adafruit_BMP085.h> // https://github.com/adafruit/Adafruit-BMP085-Library
#include <Adafruit_Sensor.h>

// ######### OBJECTS #########
Adafruit_BMP085 bmp;  //BMP085 or BMP180 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)  // BME280 0x76 | BMP280 0x76 |  BMP180 0x77 |  BMP085 0x77

// ######### VARIABLES / DEFINES / STATIC #########

// App
String AppVersion = "v2.221228";
String AppBuild = "DitroniX CWX-2-BMP180 12S SDK PCA";
String AppName = "Compact Weather Station - Domoticz";

// Variables
char SensorResult[10];
float TemperatureC;
int Pressure;
int LightMeter;

float ADC_Voltage;
float ADC_Calibration;
int Battery_Percentage;

int BatteryMeter;

// WiFi.  Setup with your Wirless Information
const char* ssid = "xxxx";      // network SSID (name)
const char* password = "xxxx";  // network password
String Hostname = "CWX-";              // Hostname Prefix
WiFiClient client;                         // Initialize the client library

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char* domoticz_server = "0.0.0.0";  // IP Address
int port = 8080;                                //Domoticz port

// Individual Domoticz Sensors
int dTemperature = 0;
int dPressure = 0;
int dLight = 0;
int dBattery = 0;

// Combined Domoticz Temp + Baro	(BMP085 I2C)
int dTHB = 30;

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5

// **************** OUTPUTS ****************
#define LED_Module 2   // Define ESP Output Port LED - Disable if using Battery to save power
#define ADC_Switch 13  // Define NLAST4599 Digital Switch

// **************** INPUTS ****************
#define ADC A0  // Define ADC (0 DC - 1V DC)

// ######### FUNCTIONS #########

// Initialise WiFi
void InitialiseWiFi() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to " + String(ssid));

    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // Stabalise for slow Access Points
    delay(5000);
    //
    // Force Hostname
    Hostname = Hostname + WiFi.macAddress().substring(WiFi.macAddress().length() - 5, WiFi.macAddress().length());
    Hostname.replace(":", "");
    WiFi.setHostname(Hostname.c_str());

    // Wifi Information
    Serial.println("WiFi SSID \t " + String(ssid)) + "(Wifi Station Mode)";
    Serial.printf("WiFi IP \t %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("WiFi GW \t %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("WiFi MASK \t %s\n", WiFi.subnetMask().toString().c_str());
    Serial.println("WiFi MAC \t " + WiFi.macAddress());
    Serial.printf("WiFi Hostname \t %s\n", WiFi.hostname().c_str());
    Serial.println("WiFi RSSI \t " + String(WiFi.RSSI()));
    Serial.println("");
  }
}

// Initialise BMP180
void InitialiseTemperatureSensor() {
  if (!bmp.begin(0x77)) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    // while (1);
  }
}

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max) {
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// Publish to Domoticz
void PublishDomoticz(int Sensor_Index, float Sensor_Value) {
  if (Sensor_Index > 0) {                         // Only Publish if Index Value
    if (client.connect(domoticz_server, port)) {  // Only Publish if Port/Connected
      Serial.print("Sending Message to Domoticz #");
      Serial.print(Sensor_Index);
      Serial.print(" ");
      Serial.println(Sensor_Value);

      client.print("GET /json.htm?type=command&param=udevice&idx=");
      client.print(Sensor_Index);

      client.print("&svalue=");
      client.print(Sensor_Value);

      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.print(domoticz_server);
      client.print(":");

      client.println(port);
      client.println("User-Agent: Arduino-ethernet");
      client.println("Connection: close");
      client.println();

      client.stop();
    } else {
      Serial.println("Not Connected/Published");
      InitialiseWiFi();
    }
  }
}

// Publish to Domoticz
void PublishDomoticzTHB(int Sensor_Index) {
  if (Sensor_Index > 0) {                         // Only Publish if Index Value
    if (client.connect(domoticz_server, port)) {  // Only Publish if Port/Connected
      Serial.print("Sending THB Message to Domoticz #");
      Serial.println(Sensor_Index);

      client.print("GET /json.htm?type=command&param=udevice&idx=");
      client.print(Sensor_Index);

      client.print("&svalue=");
      client.print(String(TemperatureC));
      client.print(";");
      client.print(String(Pressure));
      client.print(";0");

      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.print(domoticz_server);
      client.print(":");

      client.println(port);
      client.println("User-Agent: Arduino-ethernet");
      client.println("Connection: close");
      client.println();

      client.stop();
    } else {
      Serial.println("Not Connected");
      InitialiseWiFi();
    }
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

  // NLAST4599
  pinMode(ADC_Switch, OUTPUT);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  InitialiseTemperatureSensor();

  Serial.println("");
}

// ######### LOOP #########
void loop() {

  // Get Temperature ºC and round to 1 decimal place
  TemperatureC = bmp.readTemperature();
  dtostrf(TemperatureC, 5, 1, SensorResult);
  sprintf(SensorResult, "%s", SensorResult);
  Serial.print("Temperature (ºC): ");
  Serial.println(SensorResult);
  PublishDomoticz(dTemperature, int(TemperatureC));

  // Get Pressure and round to no decimal places
  Pressure = (bmp.readPressure() / 100.0F);
  Pressure = round(Pressure);
  Serial.print("Pressure (hPa): ");
  Serial.println(Pressure);
  PublishDomoticz(dPressure, int(Pressure));

  PublishDomoticzTHB(dTHB);

  // Get Light Reading
  digitalWrite(ADC_Switch, LOW);
  LightMeter = analogRead(ADC);
  Serial.print("Light: ");
  Serial.print(LightMeter);  // data output from the photoresistor (0-1024)
  Serial.println("");
  PublishDomoticz(dLight, LightMeter);

  // Read Battery or 3V3 Voltage
  digitalWrite(ADC_Switch, HIGH);  // Set ADC to Battery 3V3
  ADC_Calibration = 3.798;         // Adjust as needed for final calibration of ADC_Voltage (Battery 3V3).
  ADC_Voltage = (analogRead(ADC) * ADC_Calibration) / 1024;

  // Update the displayed info
  Serial.print("ADC Value \t");
  Serial.print(analogRead(ADC));

  Serial.print("\tADC Voltage ");
  Serial.print(ADC_Voltage);
  Serial.print(" V ");

  Battery_Percentage = MapValues(ADC_Voltage, 2.6, 3.0, 0, 100);  // Typically 2.6V Minimum and 3.0 Maximum

  Serial.print("Battery ");
  Serial.print(Battery_Percentage);
  Serial.print(" % ");

  if (Battery_Percentage > 100) {
    Serial.print("\t (Probably DC Powered)");
  }

  Serial.println("\n");
  PublishDomoticz(dBattery, ADC_Voltage);

  // Hearbeat LED (Temporary during development)
  digitalWrite(LED_Module, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_Module, HIGH);  // turn the LED off by making the voltage LOW

  // Loop Delay
  delay(1000);
}
