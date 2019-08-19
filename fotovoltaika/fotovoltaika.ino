/*HW
  mereni proudu na regulatoru
    - vstup z FW panelu, cidlo s ACS758LCB-050B +-50A
    - akumulator, cidlo s ACS712 +-20A
    - vystup, cidlo s ACS712 +-5A
    vsechna cidla jsou pripojena na vstupy AD prevodniku s ADS1115, vystup I2C SDA - D2, SCL - D1
  mereni napeti na regulatoru, spolecny je + pol, proti zemi je max na vstupu z panelu tj.20V

  náhrada
  INA3221 - 3x proudový a napěťový sensor na sběrnici I2C
  
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


const float VCC = 5.0;// supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)

Adafruit_ADS1115 ads1(0x48);  //ADDR to GND
Adafruit_ADS1115 ads2(0x49);  //ADDR to 5V

#define LCDADDRESS  0x27
#define LCDCOLS     20
#define LCDROWS     4
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(LCDADDRESS,LCDCOLS,LCDROWS);  // set the LCD
#define PRINT_SPACE  lcd.print(" ");
volatile bool showDoubleDot                 = false;
unsigned int display                        = 0;
#define DISPLAY_MAIN                         0


char                  mqtt_server[40]       = "192.168.1.56";
uint16_t              mqtt_port             = 1883;

//for LED status
Ticker ticker;

//SW name & version
#define     VERSION                          "0.25"
#define     SW_NAME                          "Fotovoltaika"

#define SEND_DELAY                           30000  //prodleva mezi poslanim dat v ms
#define SENDSTAT_DELAY                       60000 //poslani statistiky kazdou minutu
#define READADC_DELAY                        1000  //cteni ADC

#define DELAY_AFTER_ON                       300000 //interval sepnuti vystupniho rele 5 minut
unsigned long lastOffOn                      = 0; //zamezuje cyklickemu zapinani a vypinani rele

#define MAX                                  32767
#define MIN                                  -32767

#define ota
#ifdef ota
#define HOSTNAMEOTA   "fotovoltaika"
#endif

#define AUTOCONNECTNAME   HOSTNAMEOTA
#define AUTOCONNECTPWD    "password"

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

ADC_MODE(ADC_VCC);

uint32_t heartBeat                          = 0;

#ifdef ota
#include <ArduinoOTA.h>
#endif

char                  mqtt_username[40]     = "datel";
char                  mqtt_key[20]          = "hanka12";
//#define test
#ifdef test
char                  mqtt_base[60]         = "/home/SolarMereniTest";
char                  static_ip[16]         = "192.168.1.117";
#else
char                  mqtt_base[60]         = "/home/SolarMereni";
char                  static_ip[16]         = "192.168.1.116";
#endif
char                  static_gw[16]         = "192.168.1.1";
char                  static_sn[16]         = "255.255.255.0";

#define VIN                                  A0 // define the Arduino pin A0 as voltage input (V in)
#define LED2PIN                              D3 //stav rele
#define LED1PIN                              D4 //
//#define CHAROUT                              D7 //vystup z regulatoru
#define CHAROUT                              D6 //vystup z regulatoru, na testovacim je D7 asi spaleny
#define RELAYPIN                             D8 //pin rele
#define PIRPIN                               D6 //pin pir sensoru
//SDA                                        D2
//SCL                                        D1


//mereni napeti
int16_t voltageRegInMin      = MAX; //vystup z panelu, rozsah 0-20V
int16_t voltageRegOutMin     = MAX; //vystup z regulatoru, rozsah 0-15V
int16_t voltageAcuMin        = MAX; //vystup z regulatoru, rozsah 0-15V
int16_t voltage12VMin        = MAX; //vystup z regulatoru, rozsah 0-15V
int16_t voltageRegInMax      = 0; //vystup z panelu, rozsah 0-20V
int16_t voltageRegOutMax     = 0; //vystup z regulatoru, rozsah 0-15V
int16_t voltageAcuMax        = 0; //vystup z regulatoru, rozsah 0-15V
int16_t voltage12VMax        = 0; //vystup z regulatoru, rozsah 0-15V

//mereni proudu
uint16_t currentRegIn;
uint16_t currentAcu;
uint16_t currentRegOut;

Adafruit_INA219 ina219_1; //output
Adafruit_INA219 ina219_2(0x41); //batery

float                 shuntvoltage_1           = 0;
float                 busvoltage_1             = 0;
float                 current_mA_1             = 0;
float                 loadvoltage_1            = 0;
float                 power_mW_1               = 0;
float                 shuntvoltage_2           = 0;
float                 busvoltage_2             = 0;
float                 current_mA_2             = 0;
float                 loadvoltage_2            = 0;
float                 power_mW_2               = 0;


// konstanta pro přepočet naměřeného napětí na proud
// použijte 100 pro 20A verzi
int konstanta            = 185;
// proměnná pro nastavení offsetu, polovina Vcc
int offset               = 2500;

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


void setup() {
  SERIAL_BEGIN;
  DEBUG_PRINT(F(SW_NAME));
  DEBUG_PRINT(F(" "));
  DEBUG_PRINTLN(F(VERSION));

  // DEBUG_PRINT("cutOff ");
  // DEBUG_PRINTLN(cutOff);
  // DEBUG_PRINT("QOV ");
  // DEBUG_PRINTLN(QOV);
  // DEBUG_PRINT("Factor ");
  // DEBUG_PRINTLN(FACTOR);

  lcd.init();               // initialize the lcd 
  lcd.backlight();
  //lcd.begin();               // initialize the lcd 
  lcd.home();                   
  lcd.print(SW_NAME);  
  PRINT_SPACE
  lcd.print(VERSION);

  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  pinMode(CHAROUT, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  pinMode(PIRPIN, INPUT);

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
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  ads1.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads2.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);            // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);            // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);           // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);          // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);        // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads1.begin();
  ads2.begin();
  
    // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219_1.begin();
  ina219_2.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219_1.setCalibration_32V_1A();
  ina219_2.setCalibration_32V_2A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  
  //setup timers
  timer.every(SEND_DELAY, sendDataHA);
  timer.every(SENDSTAT_DELAY, sendStatisticHA);
  timer.every(READADC_DELAY, readADC);
  timer.every(500, displayTime);
  sendStatisticHA;

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(LED1PIN, HIGH);
  lcd.clear();
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
}

void relay() {
  if (digitalRead(CHAROUT)==HIGH) {
    if (millis() - DELAY_AFTER_ON > lastOffOn) {
      digitalWrite(RELAYPIN, HIGH);
      digitalWrite(LED2PIN, HIGH);
    }
  }else{
    digitalWrite(RELAYPIN, LOW);
    digitalWrite(LED2PIN, LOW);
    lastOffOn = millis();
  }
}

bool readADC(void *) {
  int16_t voltage = ads1.readADC_Differential_0_1();
  voltageRegOutMin   = min(voltage, voltageRegOutMin); 
  voltageRegOutMax   = max(voltage, voltageRegOutMax); 
  // voltage = ads1.readADC_Differential_2_3();
  // voltageRegInMin    = min(voltage, voltageRegInMin);
  // voltageRegOutMin   = min(voltage, voltageRegOutMin); 
  // voltageAcuMin      = min(voltage, voltageAcuMin);
  // voltage12VMin      = min(voltage, voltage12VMin);
  // voltageRegInMax    = max(voltage, voltageRegInMax);
  // voltageRegOutMax   = max(voltage, voltageRegOutMax); 
  // voltageAcuMax      = max(voltage, voltageAcuMax);
  // voltage12VMax      = max(voltage, voltage12VMax);

  uint16_t current = ads1.readADC_SingleEnded(0);
  currentRegIn = current;
  current = ads1.readADC_SingleEnded(1);
  currentAcu = current;
  current = ads1.readADC_SingleEnded(2);
  currentRegOut = current;
  
  readINA();

  relay();

  return true;
}

void readINA(void) {
  shuntvoltage_1 = ina219_1.getShuntVoltage_mV();
  busvoltage_1 = ina219_1.getBusVoltage_V();
  current_mA_1 = ina219_1.getCurrent_mA();
  power_mW_1 = ina219_1.getPower_mW();
  loadvoltage_1 = busvoltage_1 + (shuntvoltage_1 / 1000);
  
  Serial.println("OUTPUT:");
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage_1); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage_1); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage_1); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA_1); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW_1); Serial.println(" mW");
  Serial.println("");

  shuntvoltage_2 = ina219_2.getShuntVoltage_mV();
  busvoltage_2 = ina219_2.getBusVoltage_V();
  current_mA_2 = ina219_2.getCurrent_mA();
  power_mW_2 = ina219_2.getPower_mW();
  loadvoltage_2 = busvoltage_2 + (shuntvoltage_2 / 1000);
  
  Serial.println("OUTPUT BATERY:");
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage_2); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage_2); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage_2); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA_2); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW_2); Serial.println(" mW");
  Serial.println("");

}

bool sendDataHA(void *) {
  digitalWrite(BUILTIN_LED, LOW);
  SenderClass sender;

  // digitalWrite(BUILTIN_LED, LOW);
  // DEBUG_PRINTLN(F(" - I am sending data to HA"));
  
// //Adafruit_MQTT_Subscribe restart                = Adafruit_MQTT_Subscribe(&mqtt, MQTTBASE "restart");

  // sender.add("voltage", voltage);
  // sender.add("voltage_raw", voltage_raw);
 
  sender.add("currentRegIn",       currentRegIn);
  sender.add("currentAcu",         currentAcu);
  sender.add("currentRegOut",      currentRegOut);
  
  //napeti na regulatoru
  // sender.add("adcRegInMin", adcRegInMin);
  // sender.add("adcRegOutMin", adcRegOutMin);
  // sender.add("adcAcuMin", adcAcuMin);
  // sender.add("adc12VMin", adc12VMin);
  // sender.add("adcRegInMax", adcRegInMax);
  // sender.add("adcRegOutMax", adcRegOutMax);
  // sender.add("adcAcuMax", adcAcuMax);
  // sender.add("adc12VMax", adc12VMax);
  
  sender.add("chargerOUT",        digitalRead(CHAROUT));

  sender.add("voltageRegInMin",   voltageRegInMin);
  sender.add("voltageRegInMax",   voltageRegInMax);
  sender.add("voltageRegOutMin",  voltageRegOutMin);
  sender.add("voltageRegOutMax",  voltageRegOutMax);
  sender.add("voltageAcuMin",     voltageAcuMin);
  sender.add("voltageAcuMax",     voltageAcuMax);
  sender.add("voltage12VMin",     voltage12VMin);
  sender.add("voltage12VMax",     voltage12VMax);

  sender.add("currentRegIn",      currentRegIn);
  sender.add("currentRegOut",     currentRegOut);
  sender.add("currentAcu",        currentAcu);
  
  sender.add("busVoltage", busvoltage_1);
  sender.add("shuntVoltage", shuntvoltage_1);
  sender.add("loadVoltage", loadvoltage_1);
  sender.add("current", current_mA_1);
  sender.add("power", power_mW_1);
  
  sender.add("busVoltage_2", busvoltage_2);
  sender.add("shuntVoltage_2", shuntvoltage_2);
  sender.add("loadVoltage_2", loadvoltage_2);
  sender.add("current_2", current_mA_2);
  sender.add("power_2", power_mW_2);


  voltageRegInMin   = MAX;
  voltageRegOutMin  = MAX;
  voltageAcuMin     = MAX;   
  voltage12VMin     = MAX;   
  voltageRegInMax   = 0; 
  voltageRegOutMax  = 0;
  voltageAcuMax     = 0;   
  voltage12VMax     = 0;   
  
  DEBUG_PRINTLN(F("Calling MQTT"));

  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}

bool sendStatisticHA(void *) {
  digitalWrite(BUILTIN_LED, LOW);
  //printSystemTime();
  DEBUG_PRINTLN(F(" - I am sending statistic to HA"));

  SenderClass sender;
  sender.add("VersionSWFotovoltaika", VERSION);
  sender.add("Napeti",  ESP.getVcc());
  sender.add("HeartBeat", heartBeat++);
  sender.add("RSSI", WiFi.RSSI());
  
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
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

//display time on LCD
void lcd2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    DEBUG_WRITE('0');
  }
  DEBUG_PRINT(number);
}


void printSystemTime(){
  DEBUG_PRINT(day());
  DEBUG_PRINT(".");
  DEBUG_PRINT(month());
  DEBUG_PRINT(".");
  DEBUG_PRINT(year());
  DEBUG_PRINT(" ");
  print2digits(hour());
  DEBUG_PRINT(":");
  print2digits(minute());
  DEBUG_PRINT(":");
  print2digits(second());
}

bool displayTime(void *) {
  lcd.setCursor(TIMEX, TIMEY); //col,row
  lcd2digits(hour());
  if (showDoubleDot) {
    showDoubleDot = false;
    lcd.write(':');
  } else {
    showDoubleDot = true;
    lcd.write(' ');
  }
  lcd2digits(minute());
/*  lcd.write(':');
  lcd2digits(second());*/
  //zobrazeni hlasky o zmene uhlu kolektoru
}
#endif

//---------------------------------------------D I S P L A Y ------------------------------------------------
void lcdShow() {
  if (display==DISPLAY_MAIN) {
  }
}

