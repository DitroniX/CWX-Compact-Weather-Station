## CWX Compact IoT Weather Station (NEW for October 2024)

**IN PRODUCTION October 2024**

Powered by a single CR123 Lithium battery, this NEW CWX-3-BME280 Board is a Compact Weather Station for IoT Home Automation, providing:

 - Barometric Pressure
 - Ambient Temperature
 - Humidity
 - Ambient Light Level

![New CWX 3 - Compact Weather Station](https://github.com/DitroniX/CWX-Compact-Weather-Station/blob/main/Datasheets%20and%20Information/CWX3%20ESP32C3%20SDK%20v1.2409.101%203D%20PCA.png)

**The New CWX V3 features the following Top Level Improvements:**

 - New ESP32-C3 MCU (replacing the ESP8266/ESP12S) 
 - Type C USB Interface
 - Improved LDO Current assisting any External Devices On-Board
 - ESP32-C3 D1 UART Interface On-Board 
 - EEPROM for Data Logging and Configuration
 - User Button
 - RGB LED
 - Dallas OneWire Interface (for external temperature sensors and probes)
 - I2C External Interface Improved
 - Ambient Light Sensor
 - On-Board 32.768 kHz XTAL for Real Time Clock

**CWX3 Features**

Based on the new Espressif ESP32C3 Mini, which has both Wi-Fi and BT, the CWX3 SDK board is provided with onboard Battery Holder or External DC Input.

Interface for programming, and development, is thought a standard Type C USB, into the onboard UART and ESP32C3, so making development and use, completely self-contained.

Using the provided I2C interface, an OLED display can be plugged directly into the board, if required, for local monitoring and status information.

Opensource AQIX Firmware is provided on our GitHub, using the standard PlatformIO platform. This monitors Barometric Pressure, Temperature, Humidity, Light Level and CO2 Air Quality information, which can then be sent to your Home Automation, or cloud service, via provided MQTT or Domoticz etc., as standard.

**CWX Specification and Features**

-   **Sensors Included**
    -   **High Accuracy Barometric Sensor**  - Bosch BME 280 Module, providing: -  
	    - **Barometric Pressure**  300...1100 hPa (millibar) - Accuracy ±0.25% - which is equivalent to 1m at 400m height change - RMS Noise 0.2 Pa - which is equivalent to 1.7 cm -  
	    - **Temperature**  -40 to 85°C - Temperature coefficient offset ±1.5Pa/K. - which is equivalent to ±12.6cm at 1 °C temperature change -  
	    - **Humidity**  - 0 to 100 % - Accuracy tolerance ±3% relative humidity - Hysteresis ≤2% relative humidity
    
    -   **Ambient Light Sensor**
        -   SFH2440, providing spectral sensitivity adapted to human eye sensitivity
        -   Std. Light A; T = 2856 K Minimum 7 nA/lx
        -   Spectral range of sensitivity λ10% typically 400 to 690 nm
        -   Wavelength of max sensitivity λS max typically 620 nm
-   **MCU**
    -   **Espressif ESP32-C3 Mini**  - 2.4GHz Wi-Fi (802.11b/g/n) and Bluetooth 5
        -   ESP32-C3-MINI-1-N4  _(PCB Antenna)_
        -   ESP32-C3-MINI-1U-N4  _(External Antenna via U. FL cable)_
        -   On-Board 32.768 kHz XTAL for Real Time Clock
-   **Type C Programming / Debug**
    -   Type C USB Connector
    -   CH340K USB UART 2Mbps Transceiver
    -   Easy programming. Select ‘ESP32C3 Dev Module’
    -   BAUD rate up to 921600 to speed up Flashing
    -   Serial Monitoring Debug and Remote Logging
    -   Power Can Be Taken from USB 5V (when Flashing).
-   **EEPROM (I2C)**
    -   AT24C64 64Kbit
    -   On-board Data Logging
    -   On-board Parameters
    -   Firmware setup user parameters and configuration
-   **Battery or External DC Operation**
    -   Powered by:
        -   1 x  **CR123**  Lithium battery
        -   External DC such as USB 5V or 4.5 to 15V DC
-   **Battery Voltage Monitor**
    -   ADC provides battery voltage reading
-   **External DC Input**
    -   AMS1117 LDO allowing use of external DC input 4.5V to 15V
-   **External Temperature**
    -   Dallas OneWire Interface for External Temperatures
        -   Example Probe Sensor for Greenhouse
-   **RGB LED - Status**
    -   Configured to three GPIO’s for easy Firmware control
-   **I2C OLED Connector**
    -   Configurable 3V3 Pin 1 and 2 polarities
    -   PCB 4 Pin Header Connector
-   **Buttons**
    -   Reset
    -   User (Program or GP9 Detect)
-   **Enclosure**
    -   Designed to fit into a Waterproof Clear Cover Enclosure
-   **Compact Design and Easy to Use**
    -   Compact PCB 63mm x 51mm
    -   4 x M3 mounting holes
    -   Operating Ambient Temperature -10 to +60 °C
    -   Optional Conformal Coating – on request.


Tags: BME280, ESP32C3, SFH2440, Home Automation, Humidity, IoT, Light, Pressure, Remote Monitoring, SDK, STEM, Temperature, Domoticz, Home Assistant, ThingSpeak, Weather Station, Weather System



## **Further Information**

Additional information, and other technical details on this project, maybe found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE and PlatformIO)*
 -  **Datasheets and Information** *(Component Datasheets, Schematics, Board Layouts, Photos, Technical Documentation)*
 - **Certification** *(Related Repository Project or Part, Certification Information)*

**Repository Tabs**

 - **Wiki** *(Related Repository Wiki pages and Technical User Information)*
 - **Discussions** *(Related Repository User Discussion Forum)*
 - **Issues** *(Related Repository Technical Issues and Fixes)*

***

We value our Customers, Users of our designs and STEM Communities, all over the World . Should you have any other questions, or feedback to share to others, please feel free to:

* Visit the related [Project](https://github.com/DitroniX?tab=repositories) *plus the related* **Discussions** and **Wiki** Pages.  See tab in each separate repository.
* **Project Community Information** can be found at https://www.hackster.io/DitroniX
* [DitroniX.net Website - Contact Us](https://ditronix.net/contact/)
* **Twitter**: [https://twitter.com/DitroniX](https://twitter.com/DitroniX)
* [Supporting the STEM Projects - BuyMeACoffee](https://www.buymeacoffee.com/DitroniX)
*  **LinkedIN**: [https://www.linkedin.com/in/g8puo/](https://www.linkedin.com/in/g8puo/)

***Dave Williams, Maidstone, UK.***

Electronics Engineer | Software Developer | R&D Support | RF Engineering | Product Certification and Testing | STEM Ambassador

## STEM

**Supporting [STEM Learning](https://www.stem.org.uk/)**

Life is one long exciting learning curve, help others by setting the seed to knowledge.

![DitroniX Supporting STEM](https://hackster.imgix.net/uploads/attachments/1606838/stem_ambassador_-_100_volunteer_badge_edxfxlrfbc1_bjdqharfoe1_xbqi2KUcri.png?auto=compress%2Cformat&w=540&fit=max)
