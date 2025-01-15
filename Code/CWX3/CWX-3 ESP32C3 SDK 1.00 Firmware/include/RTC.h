/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>
#include <time.h>
#include <ESP32Time.h>

// ESP32Time rtc;
ESP32Time rtc(0); // 3600 offset in seconds GMT+1

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise RTC
void InitialiseRTC()
{
    Serial.println("Initialising RTC\n");

    // rtc.setTime(30, 24, 15, 17, 1, 2021);  // 17th Jan 2021 15:24:30
    // rtc.setTime(SS, MM, HH, DD, MM, YYYY);
    // rtc.setTime(SS, MM, HH, DD, MM, YYYY);

    // rtc.setTime(15, 14, 13, 12, 11, 1980);
    // rtc.setTime(SS, MM, HH, DD, MM, YYYY);
}

// Read RTC Time
void ReadRTC()
{
    Serial.print("RTC Time: ");
    Serial.print(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
    Serial.println("\n");

    RTC_Time = rtc.getDateTime();
}