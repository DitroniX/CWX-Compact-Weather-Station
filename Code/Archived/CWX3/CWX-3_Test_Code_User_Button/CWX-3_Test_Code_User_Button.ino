/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)
  AQIX and CWX-3 Compact Weather Station ESP32-C3 Series Board
  Features include ESP32-C3, CR123A, BME280, SFH2440 Light Sensor, SCD40, USB D1 Flashing Compatible, Battery Sensor

  PCA 1.2409.101 - Compact Weather Station - Basic Bring Up Test Code - 28th Sepember 2024

  Simplified Board Bring Up and Test GPIO User Button
  
  Remember!
  - Set the BOARD to Use ESP32C3 Dev Module (or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  - The Serial Monitor is configured for BAUD 115200
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website or github.com/DitroniX
*/

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** INPUTS ****************
#define User_Button 9  // GPIO 9

// **************** OUTPUTS ****************
#define LED_Red 10    // Red LED
#define LED_Green 18  // Gren LED
#define LED_Blue 19   // Blue LED

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1);  // 115200
  while (!Serial)
    ;
  Serial.println("");

  //  Configures the specified LED GPIO as outputs
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Blue, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, LOW);
  digitalWrite(LED_Green, LOW);
  digitalWrite(LED_Blue, LOW);

  // Configure GPIO Inputs

  pinMode(User_Button, INPUT_PULLUP);

  Serial.println("AQIX or CWX3 Configured...");
}

// **************** LOOP ****************
void loop() {

  // Heatbeat Red
  digitalWrite(LED_Red, HIGH);
  delay(1000);
  digitalWrite(LED_Red, LOW);
  delay(100);

  // Check for Button Press
  if (digitalRead(User_Button) == LOW) {

    Serial.println("Button Pressed");

    // Blue
    digitalWrite(LED_Blue, HIGH);
    delay(1000);
    digitalWrite(LED_Blue, LOW);
    delay(100);
  }
}
