/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>

#include <Adafruit_BME280.h>
// #include <Adafruit_Sensor.h>

// ######### OBJECTS #########
Adafruit_BME280 bme; // BME280 0x76 | BMP280 0x76 |  BMP180 0x77

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise BME280
void InitialiseBME280Sensor()
{
  Serial.println("Initialising BME280 Sensor");

  if (!bme.begin(BME280_Address))
  {
    Serial.println("Could not find a valid BME280 sensor");
    BME280Enabled = false;
    // while (1);
  }
} // Initialise BME280

// ReadBME280Sensor
void ReadBME280Sensor()
{
  if (BME280Enabled == true)
  {
    BME280_Pressure = bme.readPressure() / 100.0F;
    BME280_Temperature = bme.readTemperature();
    BME280_Humidity = round(bme.readHumidity());

    Serial.print("Pres mb: ");
    Serial.print(BME280_Pressure);
    Serial.print("\t");
    Serial.print("Temp C: ");
    Serial.print(BME280_Temperature,1);
    Serial.print("\t");
    Serial.print("Hum %: ");
    Serial.println(BME280_Humidity);
  }
} // ReadBME280Sensor
