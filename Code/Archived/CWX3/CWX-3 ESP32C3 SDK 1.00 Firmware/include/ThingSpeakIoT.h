/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Example Publish (play data), can be found on https://thingspeak.com/channels/2692889

// Libraries
#include <Arduino.h>
#include "ThingSpeak.h" // https://github.com/mathworks/thingspeak-arduino/blob/master/README.md

// Initialise ThingSpeak
void InitialiseThingSpeak()
{
  if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
  {
    Serial.println("ThingSpeak Enabled - Register Values Will be Published");
    ThingSpeak.begin(wlan_client); // Initialize ThingSpeak #################################################################
    Serial.println(" * Initialised ThingSpeak Client");
  }
  else
  {
    Serial.println("ThingSpeak Publishing is Disabled");
  }

} // void InitialiseThingSpeak()

// Publish a Multi-Field Formatted ThingSpeak Message
// To publish to ThingSpeak, you need a Write API Key, Channel ID and Field Number.
void SetThingSpeakField(int FieldID, float SensorValue, int dPlaces = 1, int dWidth = 5, String Sensor_Name = "")
{
  // Green LED
  digitalWrite(LED_Green, LOW);

  // Structure ThingSpeak Payload
  char MQTTPayload[10];
  dtostrf(SensorValue, dWidth, dPlaces, MQTTPayload);

  // Update Serial Monitor
  Serial.print("Sending ThingSpeak Message #");
  Serial.print(FieldID);
  Serial.print("\t");
  Serial.print(MQTTPayload);
  Serial.print(" \t");
  Serial.println(Sensor_Name);

  ThingSpeak.setField(FieldID, MQTTPayload);

  // Green LED
  digitalWrite(LED_Green, HIGH);

} // PublishThingSpeakMessage

// Publish ThingSpeak Values and Channels
void PublishThingSpeakValues(unsigned long myChannelNumber)
// void PublishThingSpeakValues(char *myWriteAPIKey, unsigned long myChannelNumber)
{
  // publish approximately every 5 sec
  if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
  {
    // Examples to Set the Fields with Values [fields 1-8 available, variable, Decimal Places]
    // Just uncomment the fields you wish to publish from this board.

    Serial.println("");

    SetThingSpeakField(1, SCD4x_CO2, 0);
    SetThingSpeakField(2, BME280_Temperature, 1);
    SetThingSpeakField(3, BME280_Humidity, 1);
    SetThingSpeakField(4, BME280_Pressure, 0);
    SetThingSpeakField(5, SFH_LightPercentage, 0);
    SetThingSpeakField(6, DS_TemperatureC[0], 1);
    SetThingSpeakField(7, DS_TemperatureC[1], 1);
    SetThingSpeakField(8, SCD4x_DewPoint, 1);

    // NTP Time and Pre-Format Example Message for Channel Status Updates
    String ThingSpeakStatus = HostName + "/" + AppVersion + " Battery: Good" + ". Last Published: " + NTP_RTC;
    // String ThingSpeakStatus = HostName + " Batt: 100%" + String(Batt_Percentage) + "%" + " Last : " + NTP_RTC;
    ThingSpeak.setStatus(ThingSpeakStatus);

    // Publish a Multi-Field Formatted ThingSpeak Channel.  Return Codes
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey1);

    if (x == 200) // OK / Success
    {
      Serial.println("ThingSpeak Channel " + String(myChannelNumber) + " Successfully Updated");
    }
    else if (x == 404) //	Incorrect API key (or invalid ThingSpeak server address)
    {
      // Red LED
      led.flash(RGBLed::RED, 20);

      Serial.println("ThingSpeak Channel " + String(myChannelNumber) + " Not Updated. Key Error (" + String(x) + ")");

      // Update OLED
      oled.clear();
      OLEDPrint("Error 404", 2, 0);
      OLEDPrint("ThingSpeak", 2, 2);
      oled.update();
      delay(1000);
    }
    else if (x == -401) //	Point was not inserted (most probable cause is the rate limit of once every 15 seconds)
    {
      // Red LED
      led.flash(RGBLed::RED, 20);

      Serial.println("ThingSpeak Channel " + String(myChannelNumber) + " Not Updated. Rate Error (" + String(x) + ")");

      // Update OLED
      oled.clear();
      OLEDPrint("Error 401", 2, 0);
      OLEDPrint("ThingSpeak", 2, 2);
      oled.update();
      delay(1000);
    }
    else
    {
      // Red LED
      led.flash(RGBLed::RED, 20);

      Serial.println("ThingSpeak Channel " + String(myChannelNumber) + " Not Updated. Other Error (" + String(x) + ")");

      // Update OLED
      oled.clear();
      OLEDPrint("Error ?", 2, 0);
      OLEDPrint("ThingSpeak", 2, 2);
      oled.update();
      delay(1000);
    }

    Serial.println("");
  }

} // PublishThingSpeakValues
