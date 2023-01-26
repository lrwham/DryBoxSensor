#ifndef CONFIG_H
#define CONFIG_H

#include "secretConfig.h"

/************************   Pins    *****************************************/
#define BATTERY_ADC_PIN A0
#define GPIO_POWER_SI7021 12

/************************ Constants *****************************************/
#define BATTERY_ADC_SAMPLES 20
#define BATTERY_ADC_SAMPLE_DELAY 5

#define DEEPSLEEP_SECONDS 60
#define DEEPSLEEP_INTERVAL 1000000 * DEEPSLEEP_SECONDS

#define SERIAL_RATE 115200

#define HEATER_ON_TIME 1000

#define MIN_ANALOG_READING 580 // 3.14V
#define MAX_ANALOG_READING 774 // 4.2V
#define MIN_BATTERY_CAPACITY 0 // 0%
#define MAX_BATTERY_CAPACITY 100 // 100%

// comment out the following lines if you are using fona or ethernet
#ifndef IO_VAR_CREATED
#define IO_VAR_CREATED
#include "AdafruitIO_WiFi.h"

#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif
#endif
#endif