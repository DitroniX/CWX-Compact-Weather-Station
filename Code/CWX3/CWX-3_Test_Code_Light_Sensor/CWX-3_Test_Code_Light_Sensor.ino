/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)
  AQIX and CWX-3 Compact Weather Station ESP32-C3 Series Board
  Features include ESP32-C3, CR123A, BME280, SFH2440 Light Sensor, SCD40, USB D1 Flashing Compatible, Battery Sensor

  PCA 1.2409.101 - Compact Weather Station - Basic Bring Up Test Code - 28th Sepember 2024

  Simplified Board Bring Up Test - Light Sensor.  Report to Serial Print.
  
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

// Libraries

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// Variables
float ADC_Voltage;
float ADC_Calibration;
int Light_Percentage;

// **************** OUTPUTS ****************
#define LED_Red 10 // Red LED
#define LED_Green 18 // Gren LED
#define LED_Blue 19 // Blue LED

// **************** INPUTS ****************
#define ADC 4  // Define ADC (0 DC - 1V DC)

// Define I2C (Expansion Port)
#define I2C_SDA 7
#define I2C_SCL 5

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

  Serial.println("AQIX or CWX3 Configured...");

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
