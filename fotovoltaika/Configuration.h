#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//SW name & version
#define     VERSION                          "0.80"
#define     SW_NAME                          "Fotovoltaika"

#define timers
#define ota
#define verbose

#define AUTOCONNECTNAME   HOSTNAMEOTA
#define AUTOCONNECTPWD    "password"

#ifdef ota
#define HOSTNAMEOTA   "fotovoltaika"
#endif

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


#define VIN                         A0 // define the Arduino pin A0 as voltage input (V in)
#define LED2PIN                     D3 //stav rele
#define LED1PIN                     D4 //
//#define CHAROUTPIN                  D7 //vystup z regulatoru, na testovacim je D7 asi spaleny
#define RELAY1PIN                   D8 //pin rele 1
#define RELAY2PIN                   D6 //pin rele 2
#define PIRPIN                      D5 //pin pir sensoru
//SDA                               D2
//SCL                               D1

char                  mqtt_server[40]       = "192.168.1.56";
uint16_t              mqtt_port             = 1883;
char                  mqtt_username[40]     = "datel";
char                  mqtt_key[20]          = "hanka12";
char                  mqtt_base[60]         = "/home/SolarMereni";
char                  static_ip[16]         = "192.168.1.116";
char                  static_gw[16]         = "192.168.1.1";
char                  static_sn[16]         = "255.255.255.0";
char         mqtt_topic_weather[25]         = "/home/Meteo/Temperature";


#define         CURRENT_UNIT     "A"
#define         VOLTAGE_UNIT     "V"
#define         POWER_UNIT       "W"
#define         MIN_UNIT         "m"
#define         AH_UNIT          "Ah"
#define         V2MV              1000.f  //prevod volt na milivolt Y(in milivolt) = X(in volt) * V2MV

#define LCDADDRESS  0x27
#define LCDCOLS     20
#define LCDROWS     4

#define POZREGIN_CURRENTX                   1
#define POZREGIN_CURRENTY                   1
#define POZREGOUT_CURRENTX                  8
#define POZREGOUT_CURRENTY                  1
#define POZREGIN_VOLTAGEX                   1
#define POZREGIN_VOLTAGEY                   2
#define POZREGOUT_VOLTAGEX                  1
#define POZREGOUT_VOLTAGEY                  2
#define POZREGIN_POWERX                     2
#define POZREGIN_POWERXY                    0
#define POZREGOUT_POWERX                    9
#define POZREGOUT_POWERXY                   0
#define RELAY_STATUSX                       17
#define RELAY_STATUSY                       3
#define KOEFX                               8
#define KOEFY                               2
#define TEMPERATURE_X                       4
#define TEMPERATURE_Y                       3
#define RUNMINTODAY_X                      14 
#define RUNMINTODAY_Y                       2
#define AHPANELTODAY_X                     14 
#define AHPANELTODAY_Y                      1

#define SEND_DELAY                           10000  //prodleva mezi poslanim dat v ms
#define SENDSTAT_DELAY                       60000  //poslani statistiky kazdou minutu
#define READADC_DELAY                        2000   //cteni ADC

#define RELAY_ON                             HIGH
#define RELAY_OFF                            LOW

#define CURRENT4ON                           3.f //3A

#define MAX                                  32767
#define MIN                                  -32767

#define         CHANNEL_REG_IN_CURRENT          2
#define         CHANNEL_REG_OUT_CURRENT         1
#define         CHANNEL_VOLTAGE_SUPPLY          3

#define         MVOLTDILEKADC1                  0.1875
#define         MVOLTDILEKADC2                  0.1875

#define         MVAMPERIN                       40.f        // 40mV = 1A
#define         MVAMPEROUT                      100.f       // 100mV = 1A


#endif
