/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX-2 Compact Weather Station ESP8266 Series Board V2
  Features include ESP8266, CR123A, BME280, BMP180, SFH2440 Light Sensor, USB D1 Flashing Compatible, Battery Sensor

  PCA 2212-202 - Compact Weather Station - Basic Bring Up Test Code

  Simplified Board Bring Up and Test the BME280 and SFH2440 Light Sensor.  Matrix Test. Battery Voltage Test.
  
  Remember!
  - Set the BOARD to ESP8266 DEV Module (Any Wemos D1 or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  .
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX

  Below example outputs from an uncalibrated board.  So ADC_Calibration = 3.798; 
  
  Example Output from USB 5V:
  Temperature (ºC):  18.5
  Humidity (%):    51
  Pressure (hPa): 981
  Light: 221
  ADC Value 	881	ADC Voltage 3.27 V Battery 166 % 	 (Probably DC Powered)

  Example Output from CR123A (No 3V3 Link)
  Temperature (ºC):  18.5
  Humidity (%):    46
  Pressure (hPa): 981
  Light: 173
  ADC Value 	832	ADC Voltage 3.09 V Battery 121 % 	 (Probably DC Powered)
*/


#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// ######### OBJECTS #########
Adafruit_BME280 bme;  //BME280 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)  // BME280 0x76 | BMP280 0x76 |  BMP180 0x77

// ######### VARIABLES / DEFINES / STATIC #########

// App
String AppVersion = "v2.230117";
String AppBuild = "DitroniX CWX-2-BME280 12S SDK PCA";
String AppName = "Compact Weather Station - Basic Bring Up Test";

// Variables
char SensorResult[10];
float TemperatureC;
float Humidity;
int Pressure;
int LightMeter;

float ADC_Voltage;
float ADC_Calibration;
int Battery_Percentage;

int BatteryMeter;

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5

// **************** OUTPUTS ****************
#define LED_Module 2   // Define ESP Output Port LED
#define ADC_Switch 13  // Define NLAST4599 Digital Switch

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
    Serial.println("Could not find a valid BME280 sensor, check wiring!  Could eqally be a BMP180?");
    // BME280 0x76 | BMP280 0x76 |  BMP180 0x77
    // while (1);
  }
}

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max) {
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
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
  digitalWrite(ADC_Switch, LOW);
  LightMeter = analogRead(ADC);
  Serial.print("Light: ");
  Serial.print(LightMeter);  // data output from the photoresistor (0-1024)
  Serial.println("");


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

  // Hearbeat LED (Temporary during development)
  digitalWrite(LED_Module, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_Module, HIGH);  // turn the LED off by making the voltage LOW

  // Loop Delay
  delay(1000);
}
