/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** FUNCTIONS AND ROUTINES ****************

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max)
{
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// Initialise ADC
void InitialiseADC()
{
  Serial.println("Initialising ADC");

  // Configure any specific ADC settings
}

// Read ADC
void ReadADC()
{
  Batt_Voltage = (analogRead(GPIO_VBAT) * ADC_Calibration) / 4095;
  Batt_Percentage = MapValues(Batt_Voltage, 2.6, BAT_Calibration, 0, 100); // CR123 typically 2.6V Minimum and 3.0 Maximum

  // Battery Maximum Limit
  if (Batt_Percentage > 100)
    Batt_Percentage = 100;

  Serial.print("Battery ");
  Serial.print(Batt_Voltage);
  Serial.print(" V\t");

  Serial.print("Battery ");
  Serial.print(Batt_Percentage);
  Serial.print(" %\t");

  if (Batt_Voltage > 3.0)
  {
    Serial.print("(Probably DC Powered)");
  }
  Serial.println();

  SFH_LightRaw = analogRead(GPIO_LIGHT);
  SFH_LightDC = (SFH_LightRaw * ADC_Calibration) / 4095;
  SFH_LightPercentage = MapValues(SFH_LightDC, 0, ADC_Calibration, 0, 100);

  // Light Maximum Limit
  if (SFH_LightPercentage > 100)
    SFH_LightPercentage = 100;

  // Display Values
  Serial.print("Light ");
  Serial.print(SFH_LightRaw);
  Serial.print(" \t");

  Serial.print("Light ");
  Serial.print(SFH_LightDC);
  Serial.print(" V\t");

  Serial.print("Light ");
  Serial.print(SFH_LightPercentage);
  Serial.print(" %");

  Serial.println();

} // ReadADC
