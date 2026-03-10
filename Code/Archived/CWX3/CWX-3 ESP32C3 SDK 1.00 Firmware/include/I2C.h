/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>
#include <Wire.h>

// **************** FUNCTIONS AND ROUTINES ****************

// Initialize I2C
void InitialiseI2C()
{
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_CLOCK); // 400kHz | Standard-mode (Sm) and Fast-mode (Fm) which can go up to 100kHz and 400kHz respectively
} // InitialiseI2C

// I2C Bus Scanner
void ScanI2CBus()
{

    byte error, address;
    int nDevices;

    Serial.println("Scanning I2C Bus for Devices for Auto Configuration ...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        nDevices++;
        if (nDevices == 1)
            Serial.println(" I2C Device(s) found at Address:");
        if (error == 0)
        {
            Serial.print(" * Hex 0x");
            Serial.print(address, HEX);

            Serial.print(" (");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.print(address);
            Serial.print(")\t");

            switch (address)
            {
            case 0x38:
                Serial.print(" Humidity/Temperature Sensor");
                break;
            case 0x48 ... 0x4B:
                Serial.print(" ADS1115 16-bit ADC");
                break;
            case 0x4C ... 0x4F:
                Serial.print(" Sensor");
                break;
            case 0x3C ... 0x3D:
                Serial.print(" OLED Display");
                OLEDEnabled = true;
                break;
            case 0x50 ... 0x5F:
                Serial.print(" EEPROM");
                EEPROMEnabled = true;
                break;
            case 0x62:
                Serial.print(" SCD4x CO2 Air Quality Sensor");
                AppAcronym = "AQIX";
                SCD4xEnabled = true;
                break;
            case 0x76 ... 0x77:
                Serial.print(" BMP/BME Temperature/Humidity/Barometric");
                BME280Enabled = true;
                break;
            }
            Serial.println();
        }
        else if (error == 4)
        {
            Serial.print(" * Unknown error at address Decimal ");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.print(address);
            Serial.print(" = Hex 0x");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0 && EEPROMEnabled == false)
    {
        Serial.println(" * No I2C devices found.");
        Serial.println(" * Possible Hardware Issue? or non " + AppAcronym + " Board?\n");
    }
    else
    {
        Serial.println(AppAcronym + " I2C Bus Scan Complete\n");
    }
    Wire.endTransmission();
} // ScanI2CBus