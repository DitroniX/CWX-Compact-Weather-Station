/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  CWX-2 Compact Weather Station ESP8266 Series Board V2
  Features include ESP8266, CR123A, BME280, SFH2440 Light Sensor, USB D1 Flashing Compatible, Battery Sensor

  PCA 2212-202 - Compact Weather Station - Basic Bring Up Test Code - 23th December 2022
  .
  Simplified Board Bring Up Test - Light Sensor.  Report to Serial Print.
  
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

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// Variables
float ADC_Voltage;
float ADC_Calibration;
int Light_Percentage;

// **************** OUTPUTS ****************
#define LED_Module 2   // Define ESP Output Port LED
#define ADC_Switch 13  // Define NLAST4599 Digital Switch

// **************** INPUTS ****************
#define ADC A0  // Define ADC (0 DC - 1V DC)

// Define I2C (Expansion Port)
#define I2C_SDA 4
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

  // LED
  pinMode(LED_Module, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Module, HIGH);

    // NLAST4599
  pinMode(ADC_Switch, OUTPUT);

  Serial.println("");

}

// ######### LOOP #########
void loop() {

  // Read Light Sensor
  digitalWrite(ADC_Switch, LOW);  // Set ADC to Light Sensor
  ADC_Calibration = 3.300;         // Adjust as needed for final calibration of ADC_Voltage.
  ADC_Voltage = (analogRead(ADC) * ADC_Calibration) / 1024;

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
  digitalWrite(LED_Module, LOW);
  delay(50);
  digitalWrite(LED_Module, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);

}
