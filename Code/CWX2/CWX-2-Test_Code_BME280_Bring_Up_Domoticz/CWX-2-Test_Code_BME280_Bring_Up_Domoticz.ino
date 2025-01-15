/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX-2 Compact Weather Station ESP8266 Series Board V2
  Features include ESP8266, CR123A, BME280, SFH2440 Light Sensor, USB D1 Flashing Compatible, Battery Sensor

  PCA 2212-202 - Compact Weather Station - Basic Bring Up Test Code - Send to Domoticz

  Simplified Board Bring Up and Test the BME280 and SFH2440 Light Sensor.  Matrix Test. Battery Voltage Test.  No Deep-Sleep Enabled. Send to Domoticz
  
  Remember!
  - Set the BOARD to ESP8266 DEV Module (Any Wemos D1 or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX

  Example Output from USB 5V:
  Temperature (ºC):  21.6
  Sending Message to Domoticz #24 21.00
  Humidity (%):    48
  Sending Message to Domoticz #25 48.00
  Pressure (hPa): 1013
  Sending Message to Domoticz #26 1013.00
  Light: 244
  Sending Message to Domoticz #27 244.00
  ADC Value 	891	ADC Voltage 3.31 V Battery 177 % 	 (Probably DC Powered)
  Sending Message to Domoticz #28 3.31    
*/

#include "Arduino.h"
#include "Wire.h"
#include <ESP8266WiFi.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// ######### OBJECTS #########
Adafruit_BME280 bme;  //BME280 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)  // BME280 0x76 | BMP280 0x76 |  BMP180 0x77

// ######### VARIABLES / DEFINES / STATIC #########

// App
String AppVersion = "v2.221226";
String AppBuild = "DitroniX CWX-2-BME280 12S SDK";
String AppName = "Compact Weather Station - Domoticz";

// Variables
char SensorResult[10];
float TemperatureC;
int Humidity;
int Pressure;
int LightMeter;

float ADC_Voltage;
float ADC_Calibration;
int Battery_Percentage;

// WiFi.  Setup with your Wirless Information
const char* ssid = "xxxx";      // network SSID (name)
const char* password = "xxxx";  // network password
String Hostname = "CWX-";              // Hostname Prefix
WiFiClient client;                         // Initialize the client library

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char* domoticz_server = "0.0.0.0";  // IP Address
int port = 8080;                                //Domoticz port

// Set these values to the Domoticz Devices Indexes (IDX)
int dTemperature = 1;
int dHumidity = 2;
int dPressure = 3;
int dLight = 4;
int dBattery = 5;
int dTHB = 6;

/*
Example Devices
25	Dummy	14069		1	  CWX-Humidity		  Humidity	LaCrosse TX3	Humidity 0 %
27	Dummy	82027		1	  CWX-Light		      Lux	Lux	410 Lux	
26	Dummy	00082026	1	CWX-Pressure		  General	Pressure	1007.0 Bar
24	Dummy	14068		1	  CWX-Temperature		Temp	LaCrosse TX3	10.0 C
29	Dummy	1406D		1	  CWX-THB			      Temp + Humidity + Baro	THB1 - BTHR918, BTHGN129
*/

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
    //    // Force Hostname
    //    Hostname = Hostname + WiFi.macAddress().substring(WiFi.macAddress().length() - 5, WiFi.macAddress().length());
    //    Hostname.replace(":", "");
    //    WiFi.setHostname(Hostname.c_str());

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

// Initialise BME280
void InitialiseTemperatureSensor() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    // while (1);
  }
}

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max) {
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// Publish to Domoticz
void PublishDomoticz(int Sensor_Index, float Sensor_Value) {
  if (client.connect(domoticz_server, port)) {
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
    Serial.println("Not Connected");
    InitialiseWiFi();
  }
}

// Publish to Domoticz
void PublishDomoticzTHB(int Sensor_Index) {
  if (client.connect(domoticz_server, port)) {
    Serial.print("Sending THB Message to Domoticz #");
    Serial.print(Sensor_Index);

    client.print("GET /json.htm?type=command&param=udevice&idx=");
    client.print(Sensor_Index);

    client.print("&svalue=");
    client.print(String(TemperatureC));
    client.print(";");
    client.print(String(Humidity));
    client.print(";0;");
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
  TemperatureC = bme.readTemperature();
  dtostrf(TemperatureC, 5, 1, SensorResult);
  sprintf(SensorResult, "%s", SensorResult);
  Serial.print("Temperature (ºC): ");
  Serial.println(SensorResult);
  PublishDomoticz(dTemperature, int(TemperatureC));

  // Get Humity % and round to no decimal places
  Humidity = round(bme.readHumidity());
  dtostrf(Humidity, 5, 0, SensorResult);
  Serial.print("Humidity (%): ");
  Serial.println(SensorResult);
  PublishDomoticz(dHumidity, int(Humidity));

  // Get Pressure and round to no decimal places
  Pressure = (bme.readPressure() / 100.0F);
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
