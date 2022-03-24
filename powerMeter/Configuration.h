#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <ArduinoJson.h>

//SW name & version
#define     VERSION                      "0.37"
#define     SW_NAME                      "Powermeter"

#define timers
#define ota
#define verbose
#define time
#define PIR
#define wifidebug

/*
--------------------------------------------------------------------------------------------------------------------------

Version history:

--------------------------------------------------------------------------------------------------------------------------
HW
ESP8266 Wemos D1
I2C display
*/

#define CONFIG_PORTAL_TIMEOUT 60 //jak dlouho zustane v rezimu AP nez se cip resetuje
#define CONNECT_TIMEOUT 5 //jak dlouho se ceka na spojeni nez se aktivuje config portal

static const char* const      mqtt_server                    = "192.168.1.56";
static const uint16_t         mqtt_port                      = 1883;
static const char* const      mqtt_username                  = "datel";
static const char* const      mqtt_key                       = "hanka12";
static const char* const      mqtt_base                      = "/home/powermeter";
static const char* const      mqtt_pip2424                   = "/home/PIP2424/pip";
static const char* const      mqtt_bojler                    = "/home/Corridor/esp07";
static const char* const      mqtt_solarEnergyMeter          = "/home/solarEnergyMeter";
static const char* const      mqtt_vytezovac                 = "/home/Switch/Bojler";
static const char* const      mqtt_topic_restart             = "restart";
static const char* const      mqtt_topic_netinfo             = "netinfo";
static const char* const      mqtt_config_portal             = "config";
static const char* const      mqtt_config_portal_stop        = "disconfig";

#define MIN_UNIT                            "m"

//keypad i2c address
#define I2CADDR                             0x20

//display
#define LCDADDRESS                          0x27
#define LCDROWS                             4
#define LCDCOLS                             20


//All of the IO pins have interrupt/pwm/I2C/one-wire support except D0.
//#define STATUS_LED                          BUILTIN_LED //status LED
#define                                     D6 //MISO                       GPIO12
#define                                     D5 //SCK                        GPIO14
#define                                     D7 //MOSI                       GPIO13
#define VYTEZOVAC_LED                       D3 //vytezovac LED              GPIO0
#define PIRPIN                              D8 //                           GPIO15
#define                                     D0 //10k Pull-down, SS          GPIO16
//SDA                                       D2 //                           GPIO4
//SCL                                       D1 //                           GPIO5
//BUILTIN_LED                               D4 //10k Pull-up, BUILTIN_LED   GPIO2

#define SENDSTAT_DELAY                      60000 //poslani statistiky kazdou minutu
#define CONNECT_DELAY                        5000 //ms

#ifdef time
#define TIMEX                               15
#define TIMEY                               0
#endif

#include <fce.h>

#endif
