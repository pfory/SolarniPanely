/*HW
  mereni proudu na regulatoru
    - vstup z FW panelu, cidlo s  ACS758LCB-050B +-50A  40mV/A
    - akumulator, cidlo s         ACS712 +-20A          100mV/A
    - vystup, cidlo s             ACS712 +-5A           185mV/A
    vsechna cidla jsou pripojena na vstupy AD prevodniku s ADS1115, vystup I2C SDA - D2, SCL - D1
  mereni napeti na regulatoru, spolecny je + pol, proti zemi je max na vstupu z panelu tj.20V

 
  display - 20x4
  ESP8266 - Wemos
  
  větev baterie - měnič
  odběr na prázdno      0,18A   2,2W
  bez čerpadla soláru   1,36A  16,3W
  s čerpadlem soláru    3,87A  46,5W
  
*/

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#include "Sender.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_INA219.h>


#define time
#ifdef time
#include <TimeLib.h>
#include <Timezone.h>
WiFiUDP EthernetUdp;
static const char     ntpServerName[]       = "tik.cesnet.cz";
//const int timeZone = 2;     // Central European Time
//Central European Time (Frankfurt, Paris)
TimeChangeRule        CEST                  = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule        CET                   = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);
unsigned int          localPort             = 8888;  // local port to listen for UDP packets
time_t getNtpTime();
#define TIMEX   0
#define TIMEY   3
#endif

#define         CURRENT_UNIT     "A"
#define         VOLTAGE_UNIT     "V"
#define         POWER_UNIT       "W"
#define         MIN_UNIT         "m"
#define         AH_UNIT          "Ah"
#define         V2MV              1000.f  //prevod volt na milivolt Y(in milivolt) = X(in volt) * V2MV

//voltage and current meassurement
Adafruit_ADS1115 ads1(0x48);  //mereni proudu ADDR to GND
Adafruit_ADS1115 ads2(0x49);  //mereni napeti ADDR to VCC
Adafruit_ADS1115 ads3(0x4B);  //mereni napeti ADDR to SCL


#define LCDADDRESS  0x27
#define LCDCOLS     20
#define LCDROWS     4
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(LCDADDRESS,LCDCOLS,LCDROWS);  // set the LCD
#define PRINT_SPACE  lcd.print(" ");
volatile bool showDoubleDot                 = false;
unsigned int display                        = 0;
#define DISPLAY_MAIN                         0

//navrhar - https://maxpromer.github.io/LCD-Character-Creator/
byte customChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

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

char                  mqtt_server[40]       = "192.168.1.56";
uint16_t              mqtt_port             = 1883;

//for LED status
Ticker ticker;

//SW name & version
#define     VERSION                          "0.80"
#define     SW_NAME                          "Fotovoltaika"

#define SEND_DELAY                           10000  //prodleva mezi poslanim dat v ms
#define SENDSTAT_DELAY                       60000  //poslani statistiky kazdou minutu
#define READADC_DELAY                        2000   //cteni ADC

float   relayONVoltage                       = 13.5;
float   relayOFFVoltage                      = 11.f;


#define RELAY_ON                             HIGH
#define RELAY_OFF                            LOW
byte relayStatus                             = RELAY_OFF;
byte manualRelay                             = 2;
#define CURRENT4ON                           3.f //3A


#define MAX                                  32767
#define MIN                                  -32767

#define ota
#ifdef ota
#define HOSTNAMEOTA   "fotovoltaika"
#include <ArduinoOTA.h>
#endif

#define AUTOCONNECTNAME   HOSTNAMEOTA
#define AUTOCONNECTPWD    "password"

#define verbose
#ifdef verbose
  #define DEBUG_PRINT(x)                     Serial.print (x)
  #define DEBUG_PRINT2(x,y)                  Serial.print (x,y)
  #define DEBUG_PRINTDEC(x)                  Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)                   Serial.println (x)
  #define DEBUG_PRINTF(x, y)                 Serial.printf (x, y)
  #define PORTSPEED 115200             
  #define DEBUG_WRITE(x)                     Serial.write (x)
  #define DEBUG_PRINTHEX(x)                  Serial.print (x, HEX)
  #define SERIAL_BEGIN                       Serial.begin(PORTSPEED)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINT2(x,y)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x, y)
  #define DEBUG_WRITE(x)
#endif 

//ADC_MODE(ADC_VCC);

uint32_t heartBeat                          = 0;


char      mqtt_username[40]         = "datel";
char      mqtt_key[20]              = "hanka12";
char      mqtt_base[60]             = "/home/SolarMereni";
char      static_ip[16]             = "192.168.1.116";
char      static_gw[16]             = "192.168.1.1";
char      static_sn[16]             = "255.255.255.0";
char      mqtt_topic_weather[25]    = "/home/Meteo/Temperature";

#define VIN                         A0 // define the Arduino pin A0 as voltage input (V in)
#define LED2PIN                     D3 //stav rele
#define LED1PIN                     D4 //
//#define CHAROUTPIN                  D7 //vystup z regulatoru, na testovacim je D7 asi spaleny
#define RELAY1PIN                   D8 //pin rele 1
#define RELAY2PIN                   D6 //pin rele 2
#define PIRPIN                      D5 //pin pir sensoru
//SDA                               D2
//SCL                               D1

//mereni napeti   
float      voltageRegInMin          = MAX; //vystup z panelu, rozsah 0-20V
float      voltageRegOutMin         = MAX; //vystup z regulatoru, rozsah 0-15V
float      voltageRegInMax          = MIN; //vystup z panelu, rozsah 0-20V
float      voltageRegOutMax         = MIN; //vystup z regulatoru, rozsah 0-15V
float      voltageSupply            = MIN;
  
uint32_t  lastReadADC               = 0;              //interval mezi ctenim sensoru

//mereni proudu
float     currentRegIn              = 0.f;   //Ampers
float     currentRegOut             = 0.f;
      
float     currentRegInSum           = 0.f;
float     currentRegOutSum          = 0.f;
int32_t   intervalMSec              = 0;

uint32_t   runMsToday               = 0;
uint32_t   lastRunStat              = 0;
float     AhPanelToday              = 0;
float     AhOutToday                = 0;


#define         CHANNEL_REG_IN_CURRENT          2
#define         CHANNEL_REG_OUT_CURRENT         1
#define         CHANNEL_VOLTAGE_SUPPLY          3

#define         MVOLTDILEKADC1                  0.1875
#define         MVOLTDILEKADC2                  0.1875

#define         MVAMPERIN                       40.f        // 40mV = 1A
#define         MVAMPEROUT                      100.f       // 100mV = 1A

Adafruit_INA219 ina219_2; //intput
Adafruit_INA219 ina219_1(0x41); //output

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  DEBUG_PRINTLN("Entered config mode");
  DEBUG_PRINTLN(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  DEBUG_PRINTLN(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

#include <timer.h>
auto timer = timer_create_default(); // create a timer with default settings
Timer<> default_timer; // save as above


void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
  digitalWrite(LED1PIN, !state);          // set pin to the opposite state
}


//MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  char * pEnd;
  long int valL;
  String val =  String();
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  for (int i=0;i<length;i++) {
    DEBUG_PRINT((char)payload[i]);
    val += (char)payload[i];
  }
  DEBUG_PRINTLN();
  
  if (strcmp(topic, "/home/SolarMereni/manualRelay")==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("set manual control relay to ");
    manualRelay = val.toInt();
    if (val.toInt()==1) {
      DEBUG_PRINTLN(F("ON"));
    } else {
      DEBUG_PRINTLN(F("OFF"));
    }
  } else if (strcmp(topic, "/home/SolarMereni/restart")==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("RESTART");
    ESP.restart();
  } else if (strcmp(topic, mqtt_topic_weather)==0) {
      DEBUG_PRINT("Temperature from Meteo: ");
      DEBUG_PRINTLN(val.toFloat());
      displayValue(TEMPERATURE_X,TEMPERATURE_Y, (int)round(val.toFloat()), false);
      lcd.write(byte(0));
      lcd.print("C");
  }

}

void printMessageToLCD(char* t, String v) {
  lcd.clear();
  lcd.print(t);
  lcd.print(": ");
  lcd.print(v);
  delay(2000);
  lcd.clear();
}


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  SERIAL_BEGIN;
  DEBUG_PRINT(F(SW_NAME));
  DEBUG_PRINT(F(" "));
  DEBUG_PRINTLN(F(VERSION));

  lcd.init();               // initialize the lcd 
  lcd.backlight();
  //lcd.begin();               // initialize the lcd 
  lcd.home();                   
  lcd.print(SW_NAME);  
  PRINT_SPACE
  lcd.print(VERSION);
  lcd.createChar(0, customChar);

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  digitalWrite(LED1PIN, HIGH); //nesviti
  digitalWrite(LED2PIN, HIGH);
  pinMode(RELAY1PIN, OUTPUT);
  pinMode(RELAY2PIN, OUTPUT);
  digitalWrite(RELAY1PIN, LOW);
  digitalWrite(RELAY2PIN, LOW);
  pinMode(PIRPIN, INPUT);

  rst_info *_reset_info = ESP.getResetInfoPtr();
  uint8_t _reset_reason = _reset_info->reason;
  DEBUG_PRINT("Boot-Mode: ");
  DEBUG_PRINTLN(_reset_reason);
  heartBeat = _reset_reason;
  
  /*
 REASON_DEFAULT_RST             = 0      normal startup by power on 
 REASON_WDT_RST                 = 1      hardware watch dog reset 
 REASON_EXCEPTION_RST           = 2      exception reset, GPIO status won't change 
 REASON_SOFT_WDT_RST            = 3      software watch dog reset, GPIO status won't change 
 REASON_SOFT_RESTART            = 4      software restart ,system_restart , GPIO status won't change 
 REASON_DEEP_SLEEP_AWAKE        = 5      wake up from deep-sleep 
 REASON_EXT_SYS_RST             = 6      external system reset 
  */
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  ticker.attach(1, tick);
  
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  
  IPAddress _ip,_gw,_sn;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);

  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
  
  DEBUG_PRINTLN(_ip);
  DEBUG_PRINTLN(_gw);
  DEBUG_PRINTLN(_sn);

  //wifiManager.setConfigPortalTimeout(60); 
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect(AUTOCONNECTNAME, AUTOCONNECTPWD)) { 
    DEBUG_PRINTLN("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
  
#ifdef time
  DEBUG_PRINTLN("Setup TIME");
  lcd.setCursor(0,1);
  lcd.print("Setup time...");
  EthernetUdp.begin(localPort);
  DEBUG_PRINT("Local port: ");
  DEBUG_PRINTLN(EthernetUdp.localPort());
  DEBUG_PRINTLN("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  
  printSystemTime();
#endif

  
  #ifdef ota
  //OTA
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(HOSTNAMEOTA);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    // String type;
    // if (ArduinoOTA.getCommand() == U_FLASH)
      // type = "sketch";
    // else // U_SPIFFS
      // type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //DEBUG_PRINTLN("Start updating " + type);
    DEBUG_PRINTLN("Start updating ");
  });
  ArduinoOTA.onEnd([]() {
   DEBUG_PRINTLN("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DEBUG_PRINTF("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DEBUG_PRINTF("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) DEBUG_PRINTLN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) DEBUG_PRINTLN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) DEBUG_PRINTLN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) DEBUG_PRINTLN("Receive Failed");
    else if (error == OTA_END_ERROR) DEBUG_PRINTLN("End Failed");
  });
  ArduinoOTA.begin();
#endif

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //ADS1015 - 12bit = 4096
  //ADS1115 - 16bit = 65536
  //                                                             ADS1015         ADS1115
  //                                                             -------         -------
  ads1.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads2.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads3.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);            // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);            // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);           // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);          // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);        // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads1.begin();
  ads2.begin();
  ads3.begin();
  
    // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219_1.begin();
  ina219_2.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219_1.setCalibration_16V_400mA();
  ina219_2.setCalibration_32V_2A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  
  //setup timers
  timer.every(SEND_DELAY, sendDataHA);
  timer.every(SENDSTAT_DELAY, sendStatisticHA);
  timer.every(READADC_DELAY, readADC);
#ifdef time  
  timer.every(500, displayTime);
#endif
  void * a;
  sendStatisticHA(a);

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(LED1PIN, HIGH);
  lcd.clear();
  lcd.setCursor(RELAY_STATUSX,RELAY_STATUSY);
  lcd.print("OFF");
}

void loop() {
  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif

  if (digitalRead(PIRPIN)==1) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  relay();
  calcStat();
  
  //nulovani statistik o pulnoci
  if (hour()==0 && runMsToday>0) {
    runMsToday = 0;
  }
}

//---------------------------------------------R E L A Y ------------------------------------------------
void relay() {
  if (manualRelay==2) {
    readINA();
    //-----------------------------------zmena 0-1--------------------------------------------
    if (relayStatus == RELAY_OFF && (voltageRegOutMin > relayONVoltage || currentRegIn > CURRENT4ON)) {
      relayStatus = RELAY_ON;
      changeRelay(relayStatus);
      sendRelayHA(1);
    //-----------------------------------zmena 1-0--------------------------------------------
    } else if (relayStatus == RELAY_ON && voltageRegOutMax <= relayOFFVoltage) { 
      relayStatus = RELAY_OFF;
      changeRelay(relayStatus);
      sendRelayHA(0);
    }
  } else if (manualRelay==1) {
      relayStatus = RELAY_ON;
      changeRelay(relayStatus);
  } else if (manualRelay==0) {
      relayStatus = RELAY_OFF;
      changeRelay(relayStatus);
  }
  dispRelayStatus();
}

bool readADC(void *) {
  readINA();

  int32_t dilkuSupply = ads1.readADC_SingleEnded(CHANNEL_VOLTAGE_SUPPLY);
  int32_t dilkuInput  = ads1.readADC_SingleEnded(CHANNEL_REG_IN_CURRENT);
  int32_t dilkuOut    = ads1.readADC_SingleEnded(CHANNEL_REG_OUT_CURRENT);
  DEBUG_PRINT("dilkuSupply:");
  DEBUG_PRINT(dilkuSupply);
  DEBUG_PRINT(", dilkuInput:");
  DEBUG_PRINT(dilkuInput);
  DEBUG_PRINT(", dilkuOut:");
  DEBUG_PRINTLN(dilkuOut);
  
  voltageSupply    = ((float)dilkuSupply * MVOLTDILEKADC1); //in mV  example 26149 * 0.1875 = 4902,938mV
  DEBUG_PRINT("voltageSupply");
  DEBUG_PRINTLN(voltageSupply);
  
  if (lastReadADC==0) {
    lastReadADC = millis();
  }
  
  uint32_t diff = millis()-lastReadADC;
  
  currentRegIn  = ((float)(dilkuInput * 2 - dilkuSupply) * MVOLTDILEKADC1) / MVAMPERIN / 2; //in Amp example ((15170 * 2 - 25852) * 0.1875) / 40 = ((30340 - 25852) * 0.1875) / 40 = 4488 * 0.1875 / 40
  currentRegInSum += currentRegIn * diff;
  DEBUG_PRINT("currentRegIn");
  DEBUG_PRINTLN(currentRegIn);
  
  currentRegOut = ((float)(dilkuOut * 2 - dilkuSupply)   * MVOLTDILEKADC1) / MVAMPEROUT / 2;
  currentRegOutSum += currentRegOut * diff;
  DEBUG_PRINT("currentRegOut");
  DEBUG_PRINTLN(currentRegOut);
  
  intervalMSec += diff;
 
  lastReadADC = millis();

 
  lcdShow();

  return true;
}

void readINA(void) {
  float loadvoltage_1 = ina219_1.getBusVoltage_V() * 2;
  
  Serial.println("Load Voltage:  "); Serial.print(loadvoltage_1); Serial.println(" V");

  float loadvoltage_2 = 12.f; //loadvoltage_1 - ina219_2.getBusVoltage_V();
  
  Serial.println("Load Voltage:  "); Serial.print(loadvoltage_2); Serial.println(" V");
  voltageRegInMin    = min(loadvoltage_2, voltageRegInMin);
  voltageRegInMax    = max(loadvoltage_2, voltageRegInMax);
  voltageRegOutMin   = min(loadvoltage_1, voltageRegOutMin); 
  voltageRegOutMax   = max(loadvoltage_1, voltageRegOutMax); 

}

bool sendDataHA(void *) {
  digitalWrite(LED1PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  SenderClass sender;

  sender.add("relayStatus",       relayStatus);
  sender.add("manualRelay",       manualRelay);
  
  if (voltageRegInMin<MAX) {
    sender.add("voltageRegInMin",   voltageRegInMin);
  }
  if (voltageRegInMax>MIN) {
    sender.add("voltageRegInMax",   voltageRegInMax);
    sender.add("powerIn",           (currentRegInSum  / (float)intervalMSec) * voltageRegInMax);
  }
  if (voltageRegOutMin<MAX) {
    sender.add("voltageRegOutMin",  voltageRegOutMin);
  }
  if (voltageRegOutMax>MIN) {
    sender.add("voltageRegOutMax",  voltageRegOutMax);
  }
  sender.add("currentRegIn",      currentRegInSum   / (float)intervalMSec);
  if (relayStatus==HIGH) {
    sender.add("currentRegOut",   currentRegOutSum  / (float)intervalMSec);
    sender.add("powerOut",        (currentRegOutSum / (float)intervalMSec) * voltageRegOutMax);
  } else {
    sender.add("currentRegOut",   0);
    sender.add("powerOut",        0);
  }
  sender.add("intervalSec",       (float)intervalMSec/1000.f);
  
  sender.add("NapetiSbernice",    voltageSupply);
  sender.add("ch0Dilky",          ads1.readADC_SingleEnded(0));
  sender.add("ch1Dilky",          ads1.readADC_SingleEnded(1));
  sender.add("ch2Dilky",          ads1.readADC_SingleEnded(2));
  sender.add("ch3Dilky",          ads1.readADC_SingleEnded(3));
  
  voltageRegInMin   = MAX;
  voltageRegOutMin  = MAX;
  voltageRegInMax   = MIN; 
  voltageRegOutMax  = MIN;
  intervalMSec      = 0;
  currentRegInSum   = 0.f;
  currentRegOutSum  = 0.f;
  
  DEBUG_PRINTLN(F("Calling MQTT"));

  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(LED1PIN, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}

bool sendStatisticHA(void *) {
  digitalWrite(LED1PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  //printSystemTime();
  DEBUG_PRINTLN(F(" - I am sending statistic to HA"));

  SenderClass sender;
  sender.add("VersionSWFotovoltaika", VERSION);
  sender.add("HeartBeat", heartBeat++);
  sender.add("RSSI", WiFi.RSSI());
  
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(LED1PIN, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}


void sendRelayHA(byte akce) {
  digitalWrite(LED1PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  SenderClass sender;
  sender.add("relayChange", akce);
 
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(LED1PIN, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---------------------------------------------D I S P L A Y ------------------------------------------------
void lcdShow() {
  if (display==DISPLAY_MAIN) {
    //01234567890123456789     //Reg In    Acu    Out  
    //P  254W  100W   50Ah     //power
    //I 10.8A 10.1A   25Ah     //current
    //U 12.6V 5.45V   120m     //voltage
    //15:23 -10°C      OFF     //hour  temperature  relay status 
    lcd.setCursor(0,0);
    lcd.print("P");
    //displayValue(POZREGIN_POWERX,POZREGIN_POWERXY, voltageRegInMax*currentRegIn, false);
    displayValue2(POZREGIN_POWERX,POZREGIN_POWERXY, voltageRegInMax*currentRegIn, 3, 0);
    lcd.print(POWER_UNIT);
    //displayValue(POZREGOUT_POWERX,POZREGOUT_POWERXY, voltageRegOutMax*currentRegOut, false);
    displayValue(POZREGOUT_POWERX,POZREGOUT_POWERXY, voltageRegOutMax*currentRegOut, 3, 0);
    lcd.print(POWER_UNIT);
    lcd.setCursor(0,1);
    lcd.print("I");
    //displayValue(POZREGIN_CURRENTX,POZREGIN_CURRENTY, currentRegIn, true);
    displayValue(POZREGIN_CURRENTX,POZREGIN_CURRENTY, currentRegIn, 2, 1);
    lcd.print(CURRENT_UNIT);
    displayValue(POZREGOUT_CURRENTX,POZREGOUT_CURRENTY, currentRegOut, 2, 1);
    lcd.print(CURRENT_UNIT);
    lcd.setCursor(0,2);
    lcd.print("U");
    //displayValue(POZREGOUT_VOLTAGEX,POZREGOUT_VOLTAGEY, voltageRegOutMax, true);
    displayValue(POZREGOUT_VOLTAGEX,POZREGOUT_VOLTAGEY, voltageRegOutMax, 2, 1);
    lcd.print(VOLTAGE_UNIT);

    lcd.setCursor(KOEFX, KOEFY);
    lcd.print(voltageSupply/V2MV, 3);
    lcd.print(VOLTAGE_UNIT);

    displayValue(RUNMINTODAY_X,RUNMINTODAY_Y, 0, false); //runMsToday / 1000 / 60, false);
    lcd.print(MIN_UNIT);

    displayValue(AHPANELTODAY_X,AHPANELTODAY_Y, 0, false);
    lcd.print(AH_UNIT);

    
  }
}


void displayValue(int x, int y, float value, bool des) {
  /*
  value     des=true   des=false
               01234        0123
 245.5         245.5         245 
  89.3          89.3          89 
  10.0          10.0          10 
   9.9           9.9           9 
   1.1           1.1           1 
   0.9           0.9           0 
   0.1           0.0           0 
   0.0           0.0           0 
  -0.1          -0.1          -0 
  -0.9          -0.9          -0 
  -1.0          -1.0          -1 
  -9.9          -9.9          -9 
 -10.0         -10.0         -10 
 -25.2         -25.2         -25 
-245.5         -245         -245
   */
  lcd.setCursor(x,y);
  
  //DEBUG_PRINTLN(F(value);
  if (!des) {
    value = round(value);
  }

  if (!des && value>-100.f) {
    lcd.print(F(" "));
  }
  
  if (value>=100.f) {
  } else if (value>=10.f && value < 100.f) {
    lcd.print(F(" "));
  } else if (value<10.f && value>=0.f) {
    //DEBUG_PRINT(F("_"));
    lcd.print(F(" "));
    lcd.print(F(" "));
  } else if (value<0.f && value>-10.f) {
    //DEBUG_PRINT(F("_"));
    lcd.print(F(" "));
    lcd.print(F("-"));
  } else if (value<-10.f) {
    lcd.print(F("-"));
    des = false;
  }
  
  lcd.print(abs((int)value));
  if (des) {
    lcd.print(F("."));
    lcd.print(abs((int)(value*10)%10));
  }
}

void displayValue2(int x, int y, float value, byte cela, byte des) {
  char buffer [18];
  if (des==0) {
    value = round(value);
  }
 
  char format[5];
  char cislo[2];
  itoa (cela, cislo, 10);
  strcpy(format, "%");
  strcat(format, cislo);
  strcat(format, "d\n");

  sprintf (buffer, format, (int)value); // send data to the buffer
  lcd.setCursor(x,y);
  lcd.print(buffer); // display line on buffer

  if (des>0) {
    lcd.print(F("."));
    lcd.print(abs((int)(value*(10*des))%(10*des)));
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_base, mqtt_username, mqtt_key)) {
      DEBUG_PRINTLN("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      // ... and resubscribe
      //client.subscribe(mqtt_base + '/' + 'inTopic');
      client.subscribe((String(mqtt_base) + "/" + "manualRelay").c_str());
      client.subscribe((String(mqtt_base) + "/" + "restart").c_str());
      client.subscribe(mqtt_topic_weather);
    } else {
      DEBUG_PRINT("failed, rc=");
      DEBUG_PRINT(client.state());
      DEBUG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void dispRelayStatus() {
  lcd.setCursor(RELAY_STATUSX,RELAY_STATUSY);
  if (relayStatus==1) {
    lcd.print(" ON");
    digitalWrite(LED2PIN, LOW);
  } else if (relayStatus==0) {
    lcd.print("OFF");
    digitalWrite(LED2PIN, HIGH);
  } else if (manualRelay==1) {
    lcd.print("MON");
    digitalWrite(LED2PIN, LOW);
  } else if (manualRelay==0) {
    lcd.print("MOF");
    digitalWrite(LED2PIN, HIGH);
  }
}


void changeRelay(byte status) {
  digitalWrite(RELAY2PIN, status);
  if (status==HIGH) {
    delay(1000);
  }
  digitalWrite(RELAY1PIN, status);
}


#ifdef time
/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  //IPAddress ntpServerIP; // NTP server's ip address
  IPAddress ntpServerIP = IPAddress(195, 113, 144, 201);

  while (EthernetUdp.parsePacket() > 0) ; // discard any previously received packets
  DEBUG_PRINTLN("Transmit NTP Request");
  // get a random server from the pool
  //WiFi.hostByName(ntpServerName, ntpServerIP);
  DEBUG_PRINT(ntpServerName);
  DEBUG_PRINT(": ");
  DEBUG_PRINTLN(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = EthernetUdp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      DEBUG_PRINTLN("Receive NTP Response");
      EthernetUdp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      DEBUG_PRINT("Seconds since Jan 1 1900 = " );
      DEBUG_PRINTLN(secsSince1900);

      // now convert NTP time into everyday time:
      DEBUG_PRINT("Unix time = ");
      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
      const unsigned long seventyYears = 2208988800UL;
      // subtract seventy years:
      unsigned long epoch = secsSince1900 - seventyYears;
      // print Unix time:
      DEBUG_PRINTLN(epoch);
	  
      TimeChangeRule *tcr;
      time_t utc;
      utc = epoch;
      
      return CE.toLocal(utc, &tcr);
      //return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  DEBUG_PRINTLN("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  EthernetUdp.beginPacket(address, 123); //NTP requests are to port 123
  EthernetUdp.write(packetBuffer, NTP_PACKET_SIZE);
  EthernetUdp.endPacket();
}

void printSystemTime(){
  char buffer[20];
  sprintf(buffer, "%02d.%02d.%4d %02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());
  DEBUG_PRINT(buffer);
}

bool displayTime(void *) {
  lcd.setCursor(TIMEX, TIMEY); //col,row
  char buffer[6];
  if (showDoubleDot) {
    sprintf(buffer, "%02d:%02d", hour(), minute());
  } else {
    sprintf(buffer, "%02d %02d", hour(), minute());
  }
  lcd.print(buffer);
  return true;
}
#endif


void calcStat() {
  uint32_t diff = millis() - lastRunStat;
  if (relayStatus == RELAY_ON) {
    runMsToday += diff;
    AhPanelToday += (currentRegOut * diff) / 1000 / 12;
  }
  
}