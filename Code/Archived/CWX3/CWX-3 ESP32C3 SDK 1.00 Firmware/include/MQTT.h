/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>
#include <PubSubClient.h> // https://pubsubclient.knolleary.net/api

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// MQTT Object / Client Instance
PubSubClient mqtt_client(wlan_client);

// **************** FUNCTIONS AND ROUTINES ****************

// Callback for Receiving MQTT Messages
void MQTTCallback(char *topic, byte *message, unsigned int length)
{
    Serial.print("MQTT Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
} // MQTTCallback

// Initialise MQTT
void InitialiseMQTT()
{
    if (EnableMQTT == true && WiFi.status() == WL_CONNECTED)
    {
        // Initialise MQTT Connection and Instance
        mqtt_client.setServer(MQTTServer, MQTTPort);

        // if (EnableMQTTCallBack == true)
        //     mqtt_client.setCallback(MQTTCallback);

        // Validate Topic Suffix
        MQTTTopic = MQTTTopicMain;
        if (MQTTTopic.charAt(MQTTTopic.length() - 1) != '/')
            MQTTTopic = MQTTTopic + "/";

        // Topic Insert Board AppAcronym or HostName
        MQTTTopic = MQTTTopic + HostName + "/";

        if (MQTTForceTopicLower == true)
            MQTTTopic.toLowerCase();

        Serial.println("Initialised MQTT - Topic = " + MQTTTopic);
        Serial.println("MQTT Enabled - Register Values Will be Published");

        // mqtt_client.subscribe("Light/Switch");
        // mqtt_client.setCallback(MQTTCallback);
    }
    else
    {
        Serial.println("MQTT Publishing is Disabled");

        // Red LED
        led.flash(RGBLed::RED, 20);
    }
} // InitialiseMQTT

// Publish a Formatted MQTT Message
void PublishMQTTMessage(String SensorName, float SensorValue, int dPlaces = 0, int dWidth = 5, int dQOS = MQTTQoS)
{

    if (EnableMQTT == true && WiFi.status() == WL_CONNECTED && mqtt_client.connect("wlan_client", MQTTUser, MQTTPassword))
    // if (mqtt_client.connected)
    {

        // Green LED
        led.flash(RGBLed::GREEN, 50);

        // Format Sensor Message to MQTTTopic as UTF-8 String
         String MQTTTopicx = MQTTTopic + SensorName;
        if (MQTTForceTopicLower == true)
            MQTTTopicx.toLowerCase();

        int str_len = MQTTTopicx.length() + 1;
        char MQTTFullTopic[str_len];
        MQTTTopicx.toCharArray(MQTTFullTopic, str_len);

        // Structure MQTT Payload
        char MQTTPayload[10];
        dtostrf(SensorValue, dWidth, dPlaces, MQTTPayload);

        // Update Serial Monitor
        Serial.print("Sending MQTT Message: ");
        Serial.print(MQTTTopicx);
        Serial.print("\t");
        Serial.println(MQTTPayload);

        // Publish
        mqtt_client.publish(MQTTFullTopic, MQTTPayload);
    }
    else
    {
        // Red LED
        led.flash(RGBLed::RED, 50);

        Serial.println("WiFi or MQTT Broker Not Connected");

        // Update OLED
        oled.clear();
        OLEDPrint("Error", 2, 0);
        OLEDPrint("MQTT", 2, 2);
        OLEDPrint("Server", 2, 4);
        oled.update();
        delay(1000);

        // Stabalise for slow Access Points
        InitialiseWiFi();
        InitialiseMQTT();
    }
} // PublishMQTTMessage

// Publish MQTT Values
void PublishMQTTValues()
{
    if (EnableMQTT == true)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            // Board Readings
            if (mqttBME280_Humidity == true)
                PublishMQTTMessage("BME280_Humidity", BME280_Humidity, 0);

            if (mqttBME280_Temperature == true)
                PublishMQTTMessage("BME280_Temperature", BME280_Temperature, 1);

            if (mqttBME280_Pressure == true)
                PublishMQTTMessage("BME280_Pressure", BME280_Pressure);

            if (mqttSCD4x_CO2 == true)
                PublishMQTTMessage("SCD4x_CO2", SCD4x_CO2);

            if (mqttSCD4x_eCO2 == true)
                PublishMQTTMessage("SCD4x_eCO2", SCD4x_eCO2);

            if (mqttSCD4x_Humidity == true)
                PublishMQTTMessage("SCD4x_Humidity", SCD4x_Humidity);

            if (mqttSCD4x_Temperature == true)
                PublishMQTTMessage("SCD4x_Temperature", SCD4x_Temperature, 1);

            if (mqttSCD4x_TemperatureOffset == true)
                PublishMQTTMessage("SCD4x_TemperatureOffset", SCD4x_TemperatureOffset, 1);

            if (mqttSCD4x_DewPoint == true)
                PublishMQTTMessage("SCD4x_DewPoint", SCD4x_DewPoint, 1);

            if (mqttSCD4x_Altitude == true)
                PublishMQTTMessage("SCD4x_Altitude", SCD4x_Altitude);

            if (mqttSFH_LightRaw == true)
                PublishMQTTMessage("SFH_LightRaw", SFH_LightRaw);

            if (mqttSFH_LightDC == true)
                PublishMQTTMessage("SFH_LightDC", SFH_LightDC, 1);

            if (mqttSFH_LightPercentage == true)
                PublishMQTTMessage("SFH_LightPercentage", SFH_LightPercentage);

            if (mqttBatt_Voltage == true)
                PublishMQTTMessage("Batt_Voltage", Batt_Voltage, 1);

            if (mqttBatt_Percentage == true)
                PublishMQTTMessage("Batt_Percentage", Batt_Percentage);

            // Publish OneWire Sensora too
            // PublishDomoticzOneWireValues();

            Serial.println(RTC_Time + " > Published to MQTT Broker\n");
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("MQTT Broker Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("MQTT", 2, 2);
            OLEDPrint("Broker", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }

} // PublishMQTTValues

// Other WIP Code
// while (!client.connected())
// {
//     Serial.println("Connecting to MQTT...");
//     if (client.connect("ESP32Client", mqttUser, mqttPassword))
//     {
//         Serial.println("connected " + String(client.state()));
//         client.subscribe(topic, "XXXX");
//     }
//     else
//     {
//         // Red LED
//         led.flash(RGBLed::RED, 20);
//         Serial.print("failed with state ");
//         Serial.print(client.state());
//         delay(2000);
//     }
// }

// Publish OneWire DS Temperature Sensors to MQTT in Degrees C.
void PublishMQTTOneWireValues()
{
    if (EnableMQTT == true)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            // Read Sensors
            ReadOneWire();

            // Loop Through DS Temperature Sensors
            for (int Sensor = 0; Sensor < SensorCount; Sensor++)
            {
                // Publish to Domoticz.  Incremented Index.
                PublishMQTTMessage("OneWire" + String(Sensor), DS_TemperatureC[Sensor], 0);
            }
        }
    }
}
