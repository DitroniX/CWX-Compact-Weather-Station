/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>
#include <RGBLed.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

RGBLed led(LED_Red, LED_Green, LED_Blue, RGBLed::COMMON_CATHODE);

// **************** FUNCTIONS AND ROUTINES ****************

// Test RGB LEDs
void LED_Test()
{
  led.flash(RGBLed::RED, 250);
  led.flash(RGBLed::GREEN, 250);
  led.flash(RGBLed::BLUE, 250);
} // LED_Test
