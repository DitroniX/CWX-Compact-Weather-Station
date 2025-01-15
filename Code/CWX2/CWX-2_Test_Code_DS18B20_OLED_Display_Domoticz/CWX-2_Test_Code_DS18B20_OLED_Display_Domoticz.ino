/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX-2 Compact Weather Station ESP8266 Series Board V2
  Features include ESP8266, CR123A, BME280, SFH2440 Light Sensor, USB D1 Flashing Compatible, Battery Sensor

  PCA 2212-202 - Compact Weather Station - Basic Bring Up Test Code - 30th May 2023

  Simplified Board Bring Up Test - OLED Display with External Temperature using DS18B20 (on User GP12), with Domoticz Publishing
  
  Remember!
  - Set the BOARD to ESP8266 DEV Module (Any Wemos D1 or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  .
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

// Libraries
#include <Wire.h>
#include <GyverOLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1;  // Loop Delay in Seconds

// DS18B20
#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// **************** OUTPUTS ****************
#define LED_Module 2  // Define ESP Output Port LED

// ****************  OLED Instance. You will need to select your OLED Display.
// Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;  // 0.6"
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSH1106_128x64> oled;

// WiFi.  Setup with your Wirless Information
const char* ssid = "";      // network SSID (name)
const char* password = "";  // network password
String Hostname = "CWX-";   // Hostname Prefix
WiFiClient client;          // Initialize the client library

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char* domoticz_server = "0.0.0.0";  // IP Address
int port = 8080;                          //Domoticz port
int domoticz_idx = 0;                     // Domoticz Device Index

// ######### FUNCTIONS #########

// Initialise WiFi
void InitialiseWiFi() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to " + String(ssid));

    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // Stabalise for slow Access Points
    delay(2000);
    //
    // Force Hostname
    Hostname = Hostname + WiFi.macAddress().substring(WiFi.macAddress().length() - 5, WiFi.macAddress().length());
    Hostname.replace(":", "");
    WiFi.setHostname(Hostname.c_str());

    // Wifi Information
    Serial.println("WiFi SSID \t " + String(ssid)) + "(Wifi Station Mode)";
    Serial.printf("WiFi IP \t %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("WiFi GW \t %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("WiFi MASK \t %s\n", WiFi.subnetMask().toString().c_str());
    Serial.println("WiFi MAC \t " + WiFi.macAddress());
    Serial.printf("WiFi Hostname \t %s\n", WiFi.hostname().c_str());
    Serial.println("WiFi RSSI \t " + String(WiFi.RSSI()));
    Serial.println("");
  }
}

// Publish to Domoticz
void PublishDomoticz(int Sensor_Index, float Sensor_Value) {
  if (client.connect(domoticz_server, port)) {
    Serial.print("Sending Message to Domoticz #");
    Serial.print(Sensor_Index);
    Serial.print(" ");
    Serial.println(Sensor_Value);

    client.print("GET /json.htm?type=command&param=udevice&idx=");
    client.print(Sensor_Index);

    client.print("&svalue=");
    client.print(Sensor_Value);

    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.print(domoticz_server);
    client.print(":");

    client.println(port);
    client.println("User-Agent: Arduino-ethernet");
    client.println("Connection: close");
    client.println();

    client.stop();
  } else {
    Serial.println("Not Connected");
    InitialiseWiFi();
  }
}


// ######### SETUP #########
void setup() {

  // Stabalise
  delay(250);

  // Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial)
    ;

  // LEDs
  pinMode(LED_Module, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Module, HIGH);

  // Locate Devices on the OneWire Bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" Devices.");

  // Report Parasite Power Requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Initialise I2C:
  Wire.begin(SDA, SCL);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(5, 0);
  oled.setScale(5);
  oled.print("CWX-2");
  oled.update();
  delay(1000);
}

// ######### LOOP #########
void loop() {

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  oled.clear();
  oled.setScale(3);
  oled.setCursor(0, 0);
  oled.print(tempC);
  oled.println(" C");
  oled.update();

  PublishDomoticz(domoticz_idx, tempC);

  // Heatbeat LED
  digitalWrite(LED_Module, LOW);
  delay(50);
  digitalWrite(LED_Module, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);
}
