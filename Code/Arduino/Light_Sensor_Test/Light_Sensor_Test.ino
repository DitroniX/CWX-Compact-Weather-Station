/*
  Dave Williams, DitroniX 2019-2026 (ditronix.net)
  CWX(C6), AQIX and AQIL Compact Weather Station and Air Quality Monitors - ESP32-C6 Series Board
  Features include ESP32-C6, BME280, SCD40/SCD41 Air Quality CO2 Sensor, SFH2440 Light Sensor, CR123A, Battery Sensor, USB ESP32C6 Dev Board  Flashing Compatible

  PCA 1.2603 - CWX(C6), AQIX and AQIL - Basic Bring Up Test Code - March 2026
 
  Remember!
  - Set the BOARD to Use ESP32C6 Dev Module (or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  - The Serial Monitor is configured for BAUD 115200
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website or github.com/DitroniX

  CWX           https://github.com/DitroniX/CWX-Compact-Weather-Station
  AQIX          https://github.com/DitroniX/AQIX-Air-Quality-Monitor-IoT-Weather-Station
  AQIL (LoRA)   https://github.com/DitroniX/AQIL-ESP32-C6-SX1262-LoRa-Air-Quality-Monitor-IoT-Weather-Station
*/

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// Variables
float ADC_Voltage;
float ADC_Calibration;
int Light_Percentage;

// **************** OUTPUTS ****************
#define LED_Red 22 // Red LED
#define LED_Green 21 // Gren LED
#define LED_Blue 23 // Blue LED

// **************** INPUTS ****************
#define ADC 3  // ADC (12-bit 0–4095 0-3.3V)

// ######### FUNCTIONS #########

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max) {
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// ######### SETUP #########
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial);
  Serial.println("");

  // LEDs
  pinMode(LED_Red, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, LOW);

  Serial.println("CWX(C6), AQIX and AQIL Configured...\n");

}

// ######### LOOP #########
void loop() {

  // Read Light Sensor
  ADC_Calibration = 3.300;         // Adjust as needed for final calibration of ADC_Voltage.
  ADC_Voltage = (analogRead(ADC) * ADC_Calibration) / 4095;

  // Update the displayed info
  Serial.print("ADC Value \t");
  Serial.print(analogRead(ADC));

  Serial.print("\tADC Voltage ");
  Serial.print(ADC_Voltage);
  Serial.print(" V ");

  Light_Percentage = MapValues(ADC_Voltage, 0, 3.3, 0, 100); // 3.3V Equaltes to approximately 0.8V (800mV) - Potential Divider

  Serial.print("Light ");
  Serial.print(Light_Percentage);
  Serial.print(" % ");

  if (Light_Percentage > 100) {
    Serial.print("\t (Bright)");
  }

  Serial.println("\n");

  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);

  // Loop Delay
  delay(LoopDelay * 1000);
}
