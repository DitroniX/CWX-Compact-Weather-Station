/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>

// ****** THIS ONEWIRE IS UNDER DEVELOPMENT - AWAITING PRODUCTION BOARD AND IS NOT CURRENTLY IN USE. ******

#include <OneWire.h>
#include <DallasTemperature.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// OneWire Bus and DS18B20 Port
OneWire ds(DOW);
DallasTemperature sensors(&ds);

// arrays to hold device address
DeviceAddress SensorAddress;

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise OneWire Bus
void Initialise_OneWire()
{
    Serial.println("Initialising OneWire Sensors");

    byte i;
    byte addr[8];
    int x;

    OneWireEnabled = false;

    // Locate OneWire Devices
    Serial.print("* Scanning for OneWire Device(s) ... ");
    sensors.begin();
    Serial.print("Found ");
    SensorCount = sensors.getDeviceCount();
    Serial.print(SensorCount, DEC);
    Serial.println(" device(s):");

    while (ds.search(addr))
    {
        OneWireEnabled = true;
        x++;
        Serial.print(" * [" + String(x) + "] ");

        for (i = 0; i < 8; i++)
        {
            if (addr[i] < 16)
            {
                Serial.print('0');
            }
            Serial.print(addr[i], HEX);
            if (i < 7)
            {
                Serial.print(":");
            }
        }
        if (OneWire::crc8(addr, 7) != addr[7])
        {
            Serial.print("(OneWire Device CRC is Not Valid!)\n");
        }

        if (addr[0] == 0x10)
        {
            Serial.print("\t(DS18S20 Family Device)\n");
        }
        else if (addr[0] == 0x28)
        {
            Serial.print("\t(DS18B20 Family Device)\n");
        }
        else
        {
            Serial.print("\t(Device Family Unknown)");
            Serial.println(addr[0], HEX);
        }
    }

    Serial.print("\n");
}

// Format Sensor Address and Print
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

// Display All DS Sensors Readings
void ReadOneWire()
{
    if (OneWireEnabled == true)
    {
        // Clear Arrays
        memset(DS_TemperatureC, 0, sizeof(DS_TemperatureC));
        memset(DS_TemperatureF, 0, sizeof(DS_TemperatureF));

        // Read Sensors
        SensorCount = sensors.getDeviceCount();
        sensors.requestTemperatures();

        // Loop Through DS Temperature Sensors
        for (int Sensor = 0; Sensor < SensorCount; Sensor++)
        {
            // Store Readings
            DS_TemperatureC[Sensor] = sensors.getTempCByIndex(Sensor);
            DS_TemperatureF[Sensor] = sensors.getTempFByIndex(Sensor);

            // Display Values
            Serial.printf("Sensor %d", Sensor + 1);

            // Check for disconnected or faulty sensor.  Zero Value.
            if (DS_TemperatureC[Sensor] < -120)
            {
                DS_TemperatureC[Sensor] = 0;
                DS_TemperatureF[Sensor] = 0;
                Serial.println(" * Possibly Disconnected or Faulty");
            }
            else
            {
                // Display Values
                Serial.print(" Temperature:\t");
                Serial.print(DS_TemperatureC[Sensor], 1);
                Serial.print(" ºC\t\t(");
                Serial.print(DS_TemperatureF[Sensor], 1);
                Serial.println(" ºF)");
            }
        }

        Serial.println("");
    }
}
