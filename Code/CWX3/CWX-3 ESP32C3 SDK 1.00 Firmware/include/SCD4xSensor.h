/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>

#include <SensirionI2CScd4x.h>  // Documents https://sensirion.github.io/python-i2c-scd/api.html

SensirionI2CScd4x scd4x;

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

uint16_t error;
char errorMessage[256];

// **************** FUNCTIONS AND ROUTINES ****************

// Print formatted Uint16 Value
void printUint16Hex(uint16_t value)
{
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
} // printUint16Hex

// Print Serial Number of SCD4x Sensor
void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2)
{
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
} // printSerialNumber

// Initialise SCD4x Sensor
void InitialiseSCD4xSensor()
{
  Serial.print("Initialising SCD4x Sensor ");

  if (SCD4xEnabled == true)
  {
    scd4x.begin(Wire);

    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error)
    {
      Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
    }

    // getSerialNumber
    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error)
    {
      Serial.print("Error trying to execute getSerialNumber(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
    }
    else
    {
      printSerialNumber(serial0, serial1, serial2);
    }

    // getSensorAltitude - The sensor altitude is set to 0 meters, by default, above sea-level.
    error = scd4x.getSensorAltitude(SCD4x_Altitude);
    if (error)
    {
      Serial.print("Error trying to execute getSensorAltitude(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
    }

    // getTemperatureOffset
    error = scd4x.getTemperatureOffset(SCD4x_TemperatureOffset);
    if (error)
    {
      Serial.print("Error trying to execute getTemperatureOffset(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
    }

    // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error)
    {
      Serial.print("Error trying to execute startPeriodicMeasurement(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
    }
  }
}

// Read SCD4x Sensor
void ReadSCD4xSensor()
{
  if (SCD4xEnabled == true)
  {
    uint16_t error;
    char errorMessage[256];

    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error)
    {
      Serial.print("Error trying to execute getDataReadyFlag(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
      return;
    }
    if (!isDataReady)
    {
      return;
    }
    error = scd4x.readMeasurement(SCD4x_CO2, SCD4x_Temperature, SCD4x_Humidity);
    if (error)
    {
      Serial.print("Error trying to execute readMeasurement(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
    }

    if (SCD4x_CO2 == 0)
    {
      Serial.println("Invalid sample detected.  Sensor Faulty?.");
    }
    else
    {
      Serial.print("CO2 ppm: ");
      Serial.print(SCD4x_CO2);
      Serial.print("\t");
      Serial.print("Temp C: ");
      Serial.print(SCD4x_Temperature, 1);
      Serial.print("\t");
      Serial.print("Hum %: ");
      Serial.println(SCD4x_Humidity, 0);

      // Dynamically Calculate DewPoint = Temperature to which air must be cooled for airborne water vapor to condense into dew.
      SCD4x_DewPoint = SCD4x_Temperature - ((100 - SCD4x_Humidity) / 5);

      // By default, the sensor altitude is set to 0 meter above sea-level.
      // Note that setting a sensor altitude on the sensor overrides any pressure compensation based on a previously set ambient pressure.
      Serial.print("Altitude M: ");
      Serial.print(SCD4x_Altitude, 0);
      Serial.print("\t");
      Serial.print("DewPoint C: ");
      Serial.print(SCD4x_DewPoint, 1);
      Serial.print("\t");

      // The temperature offset represents the difference between the measured temperature by the SCD4x and the actual ambient temperature.
      Serial.print("Temperature Offset C: ");
      Serial.println(SCD4x_TemperatureOffset);
    }
  }

} // ReadSCD4xSensor
