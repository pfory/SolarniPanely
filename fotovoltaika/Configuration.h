#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#include "Sender.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>
//#include <Adafruit_INA219.h>
#include <FS.h>          //this needs to be first
#include <DoubleResetDetector.h>      //https://github.com/khoih-prog/ESP_DoubleResetDetector

//SW name & version
#define     VERSION                          "1.02"
#define     SW_NAME                          "Fotovoltaika"

#define timers
#define ota
#define verbose

#define AUTOCONNECTNAME   HOSTNAMEOTA
#define AUTOCONNECTPWD    "password"

#ifdef ota
#include <ArduinoOTA.h>
#define HOSTNAMEOTA   "fotovoltaika"
#endif


// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 2
// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

#define CONFIG_PORTAL_TIMEOUT 60 //jak dlouho zustane v rezimu AP nez se cip resetuje
#define CONNECT_TIMEOUT 120 //jak dlouho se ceka na spojeni nez se aktivuje config portal

#ifdef verbose
  #define DEBUG_PRINT(x)                     Serial.print (x)
  #define DEBUG_PRINTDEC(x)                  Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)                   Serial.println (x)
  #define DEBUG_PRINTF(x, y)                 Serial.printf (x, y)
  #define PORTSPEED 115200             
  #define DEBUG_WRITE(x)                     Serial.write (x)
  #define DEBUG_PRINTHEX(x)                  Serial.print (x, HEX)
  #define SERIAL_BEGIN                       Serial.begin(PORTSPEED)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x, y)
  #define DEBUG_WRITE(x)
#endif 


//All of the IO pins have interrupt/pwm/I2C/one-wire support except D0.
#define STATUS_LED                          BUILTIN_LED //status LED
#define LED2PIN                             D3 //stav rele, 10k Pull-up     GPIO00
#define RELAY1PIN                           D8 //pin rele 1                 GPIO16
#define RELAY2PIN                           D6 //pin rele 2, MISO           GPIO12
#define PIRPIN                              D5 //pin pir sensoru, SCK       GPIO14
//SDA                                       D2 //                           GPIO04
//SCL                                       D1 //                           GPIO05
//BUILTIN_LED                               D4 //10k Pull-up, BUILTIN_LED   GPIO02


static const char* const      mqtt_server                    = "192.168.1.56";
static const uint16_t         mqtt_port                      = 1883;
static const char* const      mqtt_username                  = "datel";
static const char* const      mqtt_key                       = "hanka12";
static const char* const      mqtt_base                      = "/home/SolarMereni";
static const char* const      static_ip                      = "192.168.2.116";
static const char* const      static_gw                      = "192.168.2.1";
static const char* const      static_sn                      = "255.255.255.0";
static const char* const      mqtt_topic_weather             = "/home/Meteo/Temperature";
static const char* const      mqtt_topic_relay               = "relay";                     //cmnd/relay; status/relay
static const char* const      mqtt_topic_restart             = "restart";
static const char* const      mqtt_topic_relayONVoltageBig   = "relayONVoltageBig";
static const char* const      mqtt_topic_relayONVoltageSmall = "relayONVoltageSmall";
static const char* const      mqtt_topic_relayOFFVoltage     = "relayOFFVoltage";
static const char* const      mqtt_topic_forecast            = "forecast";


#define         CURRENT_UNIT     "A"
#define         VOLTAGE_UNIT     "V"
#define         POWER_UNIT       "W"
#define         MIN_UNIT         "m"
#define         AH_UNIT          "Ah"
#define         V2MV              1000.f  //prevod volt na milivolt Y(in milivolt) = X(in volt) * V2MV

#define LCDADDRESS  0x27
#define LCDCOLS     20
#define LCDROWS     4

#define CFGFILE "/config.json"

#define POZREGIN_CURRENTX                   1
#define POZREGIN_CURRENTY                   1
#define POZREGOUT_CURRENTX                  7
#define POZREGOUT_CURRENTY                  1
#define POZREGIN_VOLTAGEX                   1
#define POZREGIN_VOLTAGEY                   2
#define POZREGOUT_VOLTAGEX                  7
#define POZREGOUT_VOLTAGEY                  2
#define POZREGIN_POWERX                     2
#define POZREGIN_POWERXY                    0
#define POZREGOUT_POWERX                    8
#define POZREGOUT_POWERXY                   0
#define RELAY_STATUSX                      17
#define RELAY_STATUSY                       3
#define SYSTEM_VOLTAGEX                    16
#define SYSTEM_VOLTAGEY                     0
#define TEMPERATURE_X                      11
#define TEMPERATURE_Y                       3
#define RUNMINTODAY_X                      15 
#define RUNMINTODAY_Y                       2
#define AHPANELTODAY_X                     15 
#define AHPANELTODAY_Y                      1
#define FORECAST_X                          9 
#define FORECAST_Y                          3

#define SEND_DELAY                           10000  //prodleva mezi poslanim dat v ms
#define SENDSTAT_DELAY                       60000  //poslani statistiky kazdou minutu
#define READADC_DELAY                        2000   //cteni ADC

#define RELAY_ON                             HIGH
#define RELAY_OFF                            LOW

#define RELAY_MODE_OFF                       0
#define RELAY_MODE_ON                        1
#define RELAY_MODE_AUTO                      2

#define RELAYDELAYOFFON                      120000 //in ms = 2 min

#define CURRENT4ONBIG                        6.f //6A
#define CURRENT4ONSMALL                      4.f //4A
#define FORECASTED_LIMIT_OFF                 0.06 //kWh = 5Ah

#define MAX                                  32767
#define MIN                                  -32767

#define         CHANNEL_REG_IN_CURRENT          0
#define         CHANNEL_REG_OUT_CURRENT         1
#define         CHANNEL_VOLTAGE_SUPPLY          2

#define         MVOLTDILEKADC1                  0.1875
#define         MVOLTDILEKADC2                  0.1875

#define         KOEF_OUTPUT_VOLTAGE             -3.179
#define         KOEF_INPUT_VOLTAGE              5

#define         MVAMPERIN                       40.f        // 40mV = 1A
#define         MVAMPEROUT                      100.f       // 100mV = 1A


#endif
