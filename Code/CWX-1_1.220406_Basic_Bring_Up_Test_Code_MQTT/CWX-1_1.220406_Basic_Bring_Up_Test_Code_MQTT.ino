/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX 12S Compact Weather Station ESP8266 SDK
  Features include ESP8266, CR123A, BME280, BMP180, SFH2440 Light Sensor, UART Flashing Compatible
  
  PCA 1.2204-106 - CWX-1_1.220406_Basic_Bring_Up_Test_Code_MQTT Firmware 1.220406 - 6th April 2022
  
  CWX-1 Compact Weather Station - MQTT Basic Test Code for Domoticz - No Sleep Version - subscribe("domoticz/in")
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
 
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

// Libraries
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>   // Search PubSubClient in Library (http://pubsubclient.knolleary.net/)
#include "Wire.h" 
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// ######### OBJECTS #########
Adafruit_BME280 bme; //BME280 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)  // BME280 0x76 | BMP280 0x76 |  BMP180 0x77 

// ######### VARIABLES / DEFINES / STATIC #########

  // App
  String AppVersion = "v1.220406";
  String AppBuild = "DitroniX CWX 12S SDK PCA v2204-106";
  String AppName = "CWX Compact Weather Station - MQTT Test - No Sleep";

  // WLAN
  WiFiClient  espclient; 
  const char* ssid = "xxx";       // network SSID (name) 
  const char* password = "xxx";    // network password
  const uint32_t connectTimeoutMs = 5000; // WiFi connect timeout per AP. Increase when connecting takes longer.
  byte macaddress[6]; 

  // MQTT
  PubSubClient client(espclient);
  const uint8_t mqtt_server[] = {192, 168, 0, 10}; // MQTT Server IP Address
  const char* mqttUser = ""; // MQTT User - if applicable
  const char* mqttPassword = ""; // MQTT Password - if applicable
  unsigned long timerDelay = 5; // Every 1 mins
 
  // Variables
  char SensorResult[10];
  float SensorRAW;

  // **************** IO ****************
  
  // Define I2C (Expansion Port)
  #define I2C_SDA 4
  #define I2C_SCL 5  

  // **************** OUTPUTS **************** 
  #define LED_Status 2  // Define ESP Output Port LED
  
  // **************** INPUTS **************** 
  #define ADC A0  // Define ADC (0 DC - 1V DC)  

  // ######### FUNCTIONS #########

  // Scan I2C Devices
  void scan_i2c_devices() {
    Serial.print("Scanning I2C\t");
    byte count = 0;
    Serial.print("Found Devices: ");
    Serial.print(" Devices: ");
    for (byte i = 8; i < 120; i++)
    {
    Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0)
        {
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

  // Initialise WLAN
  void InitialiseWLAN() {

    WiFi.mode(WIFI_STA); 
    
    if(WiFi.status() != WL_CONNECTED){  // Connect or reconnect to WiFi
      Serial.println("Attempting to connect to SSID: " + String(ssid));
    
      WiFi.begin(ssid, password); 
      
      // Let Stabalise
      delay(5000);     
    
      WiFi.setAutoReconnect(true);
      WiFi.persistent(false);
      
      Serial.println("WLAN SSID \t " + String(ssid)) + "(Wifi Station Mode)";
      Serial.printf("WLAN IP \t %s\n", WiFi.localIP().toString().c_str()); 
      Serial.printf("WLAN GW \t %s\n", WiFi.gatewayIP().toString().c_str());
      Serial.printf("WLAN MASK \t %s\n", WiFi.subnetMask().toString().c_str());
      Serial.println("WLAN MAC \t " + WiFi.macAddress());
      Serial.println("");
      }
  }  

  // Initialise Temperature Sensor
  void InitialiseTemperatureSensor(){ // BME280
    if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      // while (1);
    }
  }

  // MQTT Callback
  void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
    
    for (int i = 0; i < length; i++) {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    Serial.println(); 
  }

  void ReconnectMQTT() {
  
    // WLAN
    if(WiFi.status() != WL_CONNECTED){
      InitialiseWLAN(); 
      }
  
        // Stabalise
      delay(1000);  
      
      // Initialise MQTT Connection
      client.setServer(mqtt_server, 1883);
      client.setCallback(callback);
      Serial.println("Initialised MQTT");
            
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("espclient")) {
        Serial.println("    MQTT Connected & Subscribing");
        Serial.println(""); 
        // Subscribe
        client.subscribe("domoticz/in");  //Topic
      } else {
        Serial.print("      MQTT Failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
  
  // ######### SETUP #########
  void setup() {
    
    //Stabalise
    delay(500);   
 
    // Initialize UART:
    Serial.begin(115200, SERIAL_8N1);  //115200  
    while (!Serial);
    Serial.println(""); 
    Serial.println(AppVersion + " Initialized");
    Serial.println(AppBuild);
    Serial.println(AppName);
    Serial.println(""); 

    // LED
    pinMode(LED_Status, OUTPUT);    
  
    // Initialize I2C 
    Wire.begin(I2C_SDA, I2C_SCL);
    scan_i2c_devices();    
    InitialiseTemperatureSensor();

    // WLAN
    if(WiFi.status() != WL_CONNECTED){InitialiseWLAN();}
    Serial.println(""); 
  }
  
  // ######### LOOP #########
  void loop() {

    // MQTT
    if (!client.connected()) { 
      ReconnectMQTT(); 
    } else {

      // WLAN Info
      Serial.printf("WLAN IP \t %s   MQTT Connected\n", WiFi.localIP().toString().c_str()); 
          
      // Temperature ºC and round to 1 decimal place
      SensorRAW = bme.readTemperature();
      dtostrf (SensorRAW, 5, 1, SensorResult);
      sprintf (SensorResult, "%s", SensorResult);
      Serial.print("Temperature (ºC): ");  Serial.println(SensorResult);
      client.publish("cwx/temperature", SensorResult);
          
      // Humity % and round to no decimal places
      SensorRAW = round(bme.readHumidity()); 
      dtostrf(SensorRAW,5,0,SensorResult);      
      Serial.print("Humidity (%): ");   Serial.println(SensorResult);
      client.publish("cwx/humidity", SensorResult);    
      
      // Pressure and round to no decimal places
      SensorRAW = (bme.readPressure() / 100.0F);
      dtostrf(SensorRAW,5,0,SensorResult); 
      Serial.print("Pressure (hPa): "); Serial.println(SensorResult);
      client.publish("cwx/pressure", SensorResult);  
      
      // Light Reading
      SensorRAW = analogRead(ADC);
      dtostrf(SensorRAW,5,0,SensorResult);
      Serial.print("Light: ");  Serial.print(SensorResult ); 
      Serial.println("");    
      client.publish("cwx/lightmeter", SensorResult);  
         
      Serial.println(""); 
      }
            
    // Hearbeat LED
    digitalWrite(LED_Status, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(250);                       // wait for a second
    digitalWrite(LED_Status, HIGH);    // turn the LED off by making the voltage LOW

    // Loop Delay
    delay(timerDelay * 1000);

  }
