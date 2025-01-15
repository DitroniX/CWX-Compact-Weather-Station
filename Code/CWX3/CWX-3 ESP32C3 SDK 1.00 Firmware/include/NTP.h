/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information and WIKI Information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

// Libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************


// **************** FUNCTIONS AND ROUTINES ****************

// Print Local Time Examples
void PrintLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.print("Second: ");
    Serial.println(&timeinfo, "%S");
    Serial.print("Minute: ");
    Serial.println(&timeinfo, "%M");
    Serial.print("Hour: ");
    Serial.println(&timeinfo, "%H");
    Serial.print("Day of Month: ");
    Serial.println(&timeinfo, "%d");
    Serial.print("Month: ");
    Serial.println(&timeinfo, "%B");
    Serial.print("Year: ");
    Serial.println(&timeinfo, "%Y");

    Serial.print("Day of week: ");
    Serial.println(&timeinfo, "%A");
    Serial.print("Hour (12 hour format): ");
    Serial.println(&timeinfo, "%I");
    Serial.println("Time variables");

    char timeHour[3];
    strftime(timeHour, 3, "%H", &timeinfo);
    Serial.println(timeHour);

    char timeWeekDay[10];
    strftime(timeWeekDay, 10, "%A", &timeinfo);
    Serial.println(timeWeekDay);

    Serial.println();
} // PrintLocalTime

// Initialise NTP
void InitialiseNTP()
{
    if (WiFi.status() == WL_CONNECTED && EnableAutoNTPRefresh == true)
    {
        Serial.println("Initialising NTP Server\n");

        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        // PrintLocalTime();
    }
} // Initialise NTP

// Read NTP Time
void ReadNTP()
{
    if (WiFi.status() == WL_CONNECTED && EnableAutoNTPRefresh == true)
    {

        String NTP_RTC_Buffer = NTP_RTC;

        // Could be used to set, or read, internal RTC
        // NTP_RTC = timeClient.getFormattedTime();

        Serial.println("NTP RTC Time: " + NTP_RTC + "\n");

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            return;
        }

        // PrintLocalTime();

        // Used on OLED Status
        if (NTP_RTC_Buffer == NTP_RTC)
            NTPRefreshed = false;
        else
            NTPRefreshed = true;
    }
} // ReadNTP

// Update NTP # To be updated to local RTC Updating and testing of Xtal
void UpdateNTP()
{
    if (WiFi.status() == WL_CONNECTED && EnableAutoNTPRefresh == true)
    {
        Serial.println(F("Getting time from NTP server..."));

    }
} // UpdateNTP
