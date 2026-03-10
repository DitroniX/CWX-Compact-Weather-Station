/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX 12S Compact Weather Station ESP12S SDK
  PCA v2202-105 - Test Code Firmware 1.220318 - 23rd March 2022
  
  CWX-1 Compact Weather Station Publish to ThingSpeak Example
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
 
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "Wire.h" 
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "uptime_formatter.h" // https://github.com/YiannisBourkelis/Uptime-Library

// ######### OBJECTS #########
Adafruit_BME280 bme; //BME280 connect to ESP8266 I2C (GPIO 4 = SDA, GPIO 5 = SCL)  // BME280 0x76 | BMP280 0x76 |  BMP180 0x77 

// ######### VARIABLES / DEFINES / STATIC #########

  // App
  String AppVersion = "v1.220323 DX";
  String AppBuild = "DitroniX CWX 12S PCA v2202-105";
  String AppName = "CWX Compact Weather Station";

  // WLAN
  const char* ssid = "xxxx";       // network SSID (name) 
  const char* password = "xxxx";    // network password
  const uint32_t connectTimeoutMs = 5000; // WiFi connect timeout per AP. Increase when connecting takes longer.
  WiFiClient  client;  
  byte macaddress[6]; 

  // ThingSpeak
  unsigned long myChannelNumber = xxxx; // Channel
  const char * myWriteAPIKey = "xxxx"; // Key
  unsigned long timerDelay = 1; // Every 1 mins
  
  // Variables
  char SensorResult[10];
  float SensorRAW;
  float TemperatureC;
  float Humidity;
  int Pressure;
  int LightMeter;

  // **************** IO ****************
  
  // Define I2C (Expansion Port)
  #define I2C_SDA 4
  #define I2C_SCL 5  

  // **************** OUTPUTS **************** 
  #define LED_Status 2  // Define ESP Output Port LED
  
  // **************** INPUTS **************** 
  #define ADC A0  // Define ADC (0 DC - 1V DC)  

  // ######### FUNCTIONS #########

  void scan_i2c_devices() {
  // Scan I2C Devices
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

    void InitialiseWLAN() {

    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.println("Attempting to connect to " + String(ssid));

      WiFi.begin(ssid, password); 
      
      // Let Stabalise
      delay(5000);     

      WiFi.mode(WIFI_STA); 
      WiFi.setAutoReconnect(true);
      WiFi.persistent(true);
      
      Serial.println("WLAN SSID \t " + String(ssid)) + "(Wifi Station Mode)";
      Serial.printf("WLAN IP \t %s\n", WiFi.localIP().toString().c_str()); 
      Serial.printf("WLAN GW \t %s\n", WiFi.gatewayIP().toString().c_str());
      Serial.printf("WLAN MASK \t %s\n", WiFi.subnetMask().toString().c_str());
      Serial.println("WLAN MAC \t " + WiFi.macAddress());
      Serial.println("");

      // Stabalise
      delay(5000);  
      
      // Initialise ThingSpeak Connection
      ThingSpeak.begin(client);  // Initialize ThingSpeak
      Serial.println("Initialised ThingSpeak");
          
      }
   }  

   void InitialiseTemperatureSensor(){ // BME280
    if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      // while (1);
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

    // WLAN
    InitialiseWLAN();
  
    // Initialize I2C 
    Wire.begin(I2C_SDA, I2C_SCL);
    scan_i2c_devices();    
    InitialiseTemperatureSensor();

    Serial.println(""); 
  }
  
  // ######### LOOP #########
  void loop() {

    // Get Temperature ºC and round to 1 decimal place
    SensorRAW = bme.readTemperature();
    dtostrf (SensorRAW, 5, 1, SensorResult);
    sprintf (SensorResult, "%s", SensorResult);
    Serial.print("Temperature (ºC): "); 
    Serial.println(SensorResult);
    ThingSpeak.setField(1, SensorResult);    
        
    // Get Humity % and round to no decimal places
    Humidity = round(bme.readHumidity());
    dtostrf(Humidity,5,0,SensorResult);      
    Serial.print("Humidity (%): ");
    Serial.println(SensorResult);
    ThingSpeak.setField(2, SensorResult);    
    
    // Get Pressure and round to no decimal places
    Pressure = (bme.readPressure() / 100.0F);
    Pressure = round(Pressure);
    Serial.print("Pressure (hPa): ");
    Serial.println(Pressure);
    ThingSpeak.setField(3, Pressure);    
    
    // Get Light Reading
    LightMeter = analogRead(ADC);
    Serial.print("Light: ");    
    Serial.print(LightMeter ); // data output from the photoresistor (0-1024)
    Serial.println("");    
    ThingSpeak.setField(4, LightMeter );
       
    // Update ThingSpeak
    ThingSpeak.setStatus(AppVersion + " Last: " + uptime_formatter::getUptime());
    Serial.println("System Uptime " + uptime_formatter::getUptime()); 
   
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if(x == 200)
    {
      Serial.print ("Channel ");
      Serial.println(String(myChannelNumber) + " update successful.");
      Serial.println("");
    } else {      
      Serial.println("Problem updating channel. HTTP error code " + String(x));      
      Serial.println("Restarting ESP .... ");
      ESP.restart();
    }  
          
    // Hearbeat LED
    digitalWrite(LED_Status, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(LED_Status, HIGH);    // turn the LED off by making the voltage LOW
  
    // Loop Delay
    delay(timerDelay * 1000);
  }
