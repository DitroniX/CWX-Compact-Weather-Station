/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX-2 Compact Weather Station ESP8266 Series Board V2
  Features include ESP8266, CR123A, BME280, SFH2440 Light Sensor, USB D1 Flashing Compatible, Battery Sensor

  PCA 2212-202 - Compact Weather Station - Basic Bring Up Test Code - 23th December 2022

  Simplified Board Bring Up Test - OLED Display
  
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

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds


// **************** OUTPUTS ****************
#define LED_Module 2   // Define ESP Output Port LED

// ****************  OLED Instance. You will need to select your OLED Display.
// Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled; // 0.6"
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSH1106_128x64> oled;

void setup() {

  // Stabalise
  delay(250);

  // Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial);

  // LEDs
  pinMode(LED_Module, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Module, HIGH);

// Initialise I2C:
  Wire.begin(SDA, SCL);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(18, 0);
  oled.setScale(5);
  oled.print("CWX");
  oled.update();
  delay(1000);
}

// Draw Battery
void drawBattery(byte percent) {
  oled.drawByte(0b00111100);
  oled.drawByte(0b00111100);
  oled.drawByte(0b11111111);
  for (byte i = 0; i < 100 / 8; i++) {
    if (i < (100 - percent) / 8) oled.drawByte(0b10000001);
    else oled.drawByte(0b11111111);
  }
  oled.drawByte(0b11111111);
}

void loop() {

  // Loop Delay
  delay(LoopDelay * 1000);

  oled.clear();
  oled.setScale(3);
  oled.setCursor(0, 0);
  oled.print(25);
  oled.println(" C");
  drawBattery(25);
  oled.update();

  // Heatbeat LED
  digitalWrite(LED_Module, LOW);
  delay(50);
  digitalWrite(LED_Module, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);

}
