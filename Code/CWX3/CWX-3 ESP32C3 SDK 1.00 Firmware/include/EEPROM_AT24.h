/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>
#include <serialEEPROM.h> // Reference https://github.com/argandas/serialEEPROM

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// EEPROM AT24C64 64K (8192 x 8) 32-byte page writes
serialEEPROM extEEPROM(EEPROM_Address, 8192, 32); // Address, Size, PageSize

// **************** FUNCTIONS AND ROUTINES ****************

// Read 8bit Value
byte readEEPROM(unsigned int addEEPROM)
{
  uint8_t valEEPROM = 0x00;
  valEEPROM = extEEPROM.read(addEEPROM);
  return valEEPROM;
} // readEEPROM

// Write 8bit Value
void WriteEEPROM(unsigned int addEEPROM, uint8_t valEEPROM)
{
  extEEPROM.write(addEEPROM, valEEPROM);
} // WriteEEPROM

// Read 16bit Value
uint16_t readEEPROM16(unsigned int addEEPROM)
{
  uint16_t valEEPROM = 0x0000;
  valEEPROM = (extEEPROM.read(addEEPROM + 1) << 8);
  valEEPROM |= extEEPROM.read(addEEPROM);
  return valEEPROM;
} // readEEPROM16

// Write 16bit Value
void WriteEEPROM16(unsigned int addEEPROM, uint16_t valEEPROM)
{
  extEEPROM.write(addEEPROM, valEEPROM);
  extEEPROM.write(addEEPROM + 1, valEEPROM >> 8);
} // WriteEEPROM16

// Read Float Value
void readEEPROMFloat(unsigned int addEEPROM, float *valEEPROM)
{
  byte ByteArray[4];
  for (int x = 0; x < 4; x++)
  {
    // ByteArray[x] = EEPROM.read((MemPos * 4) + x);
    ByteArray[x] = extEEPROM.read((addEEPROM * 4) + x);
  }
  memcpy(valEEPROM, ByteArray, 4);
} // readEEPROMFloat

// Write Float Value
void WriteEEPROMFloat(unsigned int addEEPROM, float *valEEPROM)
{
  byte ByteArray[4];
  memcpy(ByteArray, valEEPROM, 4);
  for (int x = 0; x < 4; x++)
  {
    extEEPROM.write((addEEPROM * 4) + x, ByteArray[x]);
  }
} // WriteEEPROMFloat

// Update Progress Bar to Serial Monitor
void DisplayProgress(int Progress)
{
  if ((Progress % 256) == 0 && Progress != 0)
    Serial.printf("%c", 126);

  if ((Progress % 1024) == 0)
    Serial.printf("%c", 124);
} // DisplayProgress

// Clear EEPROM (Only if unlocked and fully clear, if Validation not correct or corruption)
void ClearEEPROM() // Size 0x2000 8192
{
  if (EEPROMEnabled == true)
  {
    if (extEEPROM.read(1) != 0x99) // Check Locked Status - Future Use
    {

      // Update OLED
      oled.clear();
      OLEDPrint(AppAcronym, 4, 0);
      OLEDPrint("EEPROM ", 2, 4);
      OLEDPrint("Formatting", 2, 6);
      oled.update();

      if (extEEPROM.read(0) != 0x20 || extEEPROM.read(8192) != 0x20) // Check Validation(Future Use) and End Byte
      {
        Serial.println("\nPreparing New EEPROM.  Full Formatting...  Please Wait...");

        for (int i = 0; i < 8192; i++)
        {
          extEEPROM.write(i, 0x00);
          DisplayProgress(i);
        }
      }
      else
      {
        Serial.println("\nFormatting (Smart Clearing) EEPROM.  Checking, Please Wait... ");
        for (int i = 1; i < 8192; i++)
        {
          if (extEEPROM.read(i) != 0x00) // Only Clear Used Bytes
          {
            Serial.print(extEEPROM.read(i), HEX);
            extEEPROM.write(i, 0x00);
            Serial.print("+");
          }
        }
      }

      WriteEEPROM(0x00, 0x20); // Write Status Byte
      {
        for (int i = 1; i < 8192; i++)
        {
          if (extEEPROM.read(i) != 0x00) // Only Clear Used Bytes
          {
            Serial.print(extEEPROM.read(i), HEX);
          }

          DisplayProgress(i);
        }
      }
      Serial.println("\nEEPROM Cleared and Ready");
    }

    else
      Serial.println("EEPROM Locked");
  }
} // ClearEEPROM

// Initialize EEPROM - Address 0x0000 = EEPROM Status
void InitialiseEEPROM()
{
  if (EEPROMEnabled == true)
  {
    /* Initialize the I2C interface and EEPROM */
    Wire.begin();

    // Format New EEPROM
    if (extEEPROM.read(0) != 0x20)
      ClearEEPROM();

    Serial.print("\nEEPROM Check: ");
    Serial.print(readEEPROM16(0x00), HEX);

    // EEPROM Validation
    if (readEEPROM16(0x00) == 0x00)
    {
      Serial.print("\t(Formatted OK)");
    }
    if (readEEPROM16(0x00) == 0xFF)
    {
      Serial.print("\t(Unformatted)");
    }
    if (readEEPROM16(0x00) == 0x20)
    {
      Serial.print("\t(Validation OK)");
    }
    if (readEEPROM16(0x01) == 0x99)
    {
      Serial.print("\t(Locked)");
    }

    Serial.println("\n");
  }
} // InitialiseEEPROM
