/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise Domoticz
void InitialiseDomoticz()
{
    // Domoticz Integration Status
    if (EnableDomoticz == true && WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Domoticz Enabled - Register Values Will be Published");
    }
    else
    {
        Serial.println("Domoticz Publishing is Disabled");
    }
}

// Publish to Domoticz - Single String Values
void PublishDomoticzString(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{
    if (Sensor_Index > 0 && EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            led.flash(RGBLed::GREEN, 50);

            Serial.print("Sending Domoticz Message #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t\t");
            Serial.println(Sensor_Name);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            wlan_client.print("&svalue=");
            wlan_client.print(Sensor_Value);

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            OLEDPrint("Server", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticz

// Publish to Domoticz - Single Numeric Values
void PublishDomoticzNumeric(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{
    if (Sensor_Index > 0 && EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            led.flash(RGBLed::GREEN, 50);

            Serial.print("Sending Domoticz Message #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t");
            Serial.println(Sensor_Name);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            wlan_client.print("&nvalue=");
            wlan_client.print(Sensor_Value);

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            OLEDPrint("Server", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzNumeric

// Publish OneWire Sensors to Domoticz
void PublishDomoticzOneWireValues()
{
    if (EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort) && WiFi.status() == WL_CONNECTED)
        {
            // Read Sensors
            SensorCount = sensors.getDeviceCount(); // Or use DomoticzOneWireMax
            sensors.requestTemperatures();

            // Loop Through DS Temperature Sensors
            for (int Sensor = 0; Sensor < (DomoticzOneWireMax); Sensor++)
            {
                // Publish to Domoticz.  Incremented Index.
                PublishDomoticzString(idxDomoticzOneWireBase + Sensor, DS_TemperatureC[Sensor], "DS Sensor"); // Degress C
                // PublishDomoticzString(idxDomoticzOneWireBase + Sensor, DS_TemperatureF[Sensor],"DS Sensor"); //  Degress F
            }
        }
    }
}

// Publish to Domoticz - Listed Values
void PublishDomoticzValues()
{
    if (EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort) && WiFi.status() == WL_CONNECTED)
        {
            // Board Readings
            if (idxBME280_Humidity > 0)
                PublishDomoticzNumeric(idxBME280_Humidity, BME280_Humidity, "BME280_Humidity");

            if (idxBME280_Temperature > 0)
                PublishDomoticzString(idxBME280_Temperature, BME280_Temperature, "BME280_Temperature");

            if (idxBME280_Pressure > 0)
                PublishDomoticzString(idxBME280_Pressure, BME280_Pressure, "BME280_Pressure");

            if (idxSCD4x_CO2 > 0)
                PublishDomoticzNumeric(idxSCD4x_CO2, SCD4x_CO2, "SCD4x_CO2");

            if (idxSCD4x_eCO2 > 0)
                PublishDomoticzString(idxSCD4x_eCO2, SCD4x_eCO2, "SCD4x_eCO2");

            if (idxSCD4x_Humidity > 0)
                PublishDomoticzNumeric(idxSCD4x_Humidity, SCD4x_Humidity, "SCD4x_Humidity");

            if (idxSCD4x_Temperature > 0)
                PublishDomoticzString(idxSCD4x_Temperature, SCD4x_Temperature, "SCD4x_Temperature");

            if (idxSCD4x_TemperatureOffset > 0)
                PublishDomoticzString(idxSCD4x_TemperatureOffset, SCD4x_TemperatureOffset, "SCD4x_TemperatureOffset");

            if (idxSCD4x_DewPoint > 0)
                PublishDomoticzString(idxSCD4x_DewPoint, SCD4x_DewPoint, "SCD4x_DewPoint");

            if (idxSCD4x_Altitude > 0)
                PublishDomoticzString(idxSCD4x_Altitude, SCD4x_Altitude, "SCD4x_Altitude");

            if (idxSFH_LightRaw > 0)
                PublishDomoticzString(idxSFH_LightRaw, SFH_LightRaw, "SFH_LightRaw");

            if (idxSFH_LightDC > 0)
                PublishDomoticzString(idxSFH_LightDC, SFH_LightDC, "SFH_LightDC");

            if (idxSFH_LightPercentage > 0)
                PublishDomoticzString(idxSFH_LightPercentage, SFH_LightPercentage, "SFH_LightPercentage");

            if (idxBatt_Voltage > 0)
                PublishDomoticzString(idxBatt_Voltage, Batt_Voltage, "Batt_Voltage");

            if (idxBatt_Percentage > 0)
                PublishDomoticzString(idxBatt_Percentage, Batt_Percentage, "Batt_Percentage");

            // Publish OneWire Sensora too
            PublishDomoticzOneWireValues();

            Serial.println(RTC_Time + " > Published to Domoticz");
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            OLEDPrint("Server", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzValues

// It is possible to post a Group of Values to a single Virtual Sensor and graph it (i.e. Voltage, Current, Wattage).
// This will require some coding in Domoticz (possible plugin/sensor type).  Feedback welcomed!.
// Publish to Domoticz EXAMPLE - Batch or Group Values Example to Virtual Sensor.  Update as needed.  Future WIP Option.
void PublishDomoticzATM(int Sensor_Index)
{
    if (Sensor_Index > 0 && EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort) && EnableDomoticz == true && WiFi.status() == WL_CONNECTED)
        {
            // Green LED
            led.flash(RGBLed::GREEN, 50);

            Serial.print("Sending ATM Group Message to Domoticz #");
            Serial.print(Sensor_Index);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            // Code here

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzATM
