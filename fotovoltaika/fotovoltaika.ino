#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float VCC = 5.0;// supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)

#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

char                  mqtt_server[40]       = "192.168.1.56";
uint16_t              mqtt_port             = 1883;
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#include "Sender.h"

//for LED status
#include <Ticker.h>
Ticker ticker;

//SW name & version
#define     VERSION                       "0.13"
#define     SW_NAME                       "Fotovoltaika"

#define SEND_DELAY                           30000  //prodleva mezi poslanim dat v ms
#define SENDSTAT_DELAY                       60000 //poslani statistiky kazdou minutu
#define READADC_DELAY                        1000  //cteni ADC

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


uint32_t heartBeat                          = 0;

#ifdef ota
#include <ArduinoOTA.h>
#endif

char                  mqtt_username[40]     = "datel";
char                  mqtt_key[20]          = "hanka12";
char                  mqtt_base[60]         = "/home/SolarMereni";
char                  static_ip[16]         = "192.168.1.116";
char                  static_gw[16]         = "192.168.1.1";
char                  static_sn[16]         = "255.255.255.0";

#define LEDPIN                               D4
#define CHARIN                               D7
#define CHAROUT                              D6
#define CHARACCU                             D5

float cutOffLimit = 1.0;// reading cutt off current. 1.00 is 1 Amper

/*
          "ACS758LCB-050B",// for model use 0
          "ACS758LCB-050U",// for model use 1
          "ACS758LCB-100B",// for model use 2
          "ACS758LCB-100U",// for model use 3
          "ACS758KCB-150B",// for model use 4
          "ACS758KCB-150U",// for model use 5
          "ACS758ECB-200B",// for model use 6
          "ACS758ECB-200U"// for model use  7   
sensitivity array is holding the sensitivy of the  ACS758
current sensors. Do not change.          
*/
float sensitivity[] ={
          40.0,// for ACS758LCB-050B
          60.0,// for ACS758LCB-050U
          20.0,// for ACS758LCB-100B
          40.0,// for ACS758LCB-100U
          13.3,// for ACS758KCB-150B
          16.7,// for ACS758KCB-150U
          10.0,// for ACS758ECB-200B
          20.0,// for ACS758ECB-200U     
         }; 

/*         
 *   quiescent Output voltage is factor for VCC that appears at output       
 *   when the current is zero. 
 *   for Bidirectional sensor it is 0.5 x VCC
 *   for Unidirectional sensor it is 0.12 x VCC
 *   for model ACS758LCB-050B, the B at the end represents Bidirectional (polarity doesn't matter)
 *   for model ACS758LCB-100U, the U at the end represents Unidirectional (polarity must match)
 *    Do not change.
 */
float quiescent_Output_voltage [] ={
          0.5,// for ACS758LCB-050B
          0.12,// for ACS758LCB-050U
          0.5,// for ACS758LCB-100B
          0.12,// for ACS758LCB-100U
          0.5,// for ACS758KCB-150B
          0.12,// for ACS758KCB-150U
          0.5,// for ACS758ECB-200B
          0.12,// for ACS758ECB-200U            
          };
const float FACTOR = sensitivity[model]/1000;// set sensitivity for selected model 0.04
const float QOV =   quiescent_Output_voltage[model] * VCC;// set quiescent Output voltage for selected model 0.5*5 = 2.5
float voltage;// internal variable for voltage
float cutOff = FACTOR/cutOffLimit;// convert current cut off to mV 0.04


//mereni napeti
int16_t adcRegInMin      = MAX; //vystup z panelu, rozsah 0-20V
int16_t adcRegOutMin     = MAX; //vystup z regulatoru, rozsah 0-15V
int16_t adcAcuMin        = MAX; //vystup z regulatoru, rozsah 0-15V
int16_t adc12VMin        = MAX; //vystup z regulatoru, rozsah 0-15V
int16_t adcRegInMax      = 0; //vystup z panelu, rozsah 0-20V
int16_t adcRegOutMax     = 0; //vystup z regulatoru, rozsah 0-15V
int16_t adcAcuMax        = 0; //vystup z regulatoru, rozsah 0-15V
int16_t adc12VMax        = 0; //vystup z regulatoru, rozsah 0-15V

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
  digitalWrite(LEDPIN, !state);          // set pin to the opposite state
}


void setup() {
  SERIAL_BEGIN;
  DEBUG_PRINT(F(SW_NAME));
  DEBUG_PRINT(F(" "));
  DEBUG_PRINTLN(F(VERSION));

  DEBUG_PRINT("cutOff ");
  DEBUG_PRINTLN(cutOff);
  DEBUG_PRINT("QOV ");
  DEBUG_PRINTLN(QOV);
  DEBUG_PRINT("Factor ");
  DEBUG_PRINTLN(FACTOR);
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(CHAROUT, INPUT);
  pinMode(CHARIN, INPUT);
  pinMode(CHARACCU, INPUT);

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

  //DEBUG_PRINTLN("Getting single-ended readings from AIN0..3");
  //DEBUG_PRINTLN("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  DEBUG_PRINTLN("Getting differential reading from AIN0 (P) and AIN1 (N)");
  DEBUG_PRINTLN("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  ads.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  //ads.setGain(GAIN_ONE);            // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);           // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);          // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);         // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);       // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.begin();
   
  //setup timers
  timer.every(SEND_DELAY, sendDataHA);
  timer.every(SENDSTAT_DELAY, sendStatisticHA);
  timer.every(READADC_DELAY, readADC);
  sendStatisticHA;

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(LEDPIN, HIGH);

}

void loop() {
  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif
}

bool readADC(void *) {
  int16_t adc;
  // int16_t adc[4];
  // for (byte i=0; i<4; i++) {
    // adc[i] = ads.readADC_SingleEnded(i);
    // DEBUG_PRINT("AIN");
    // DEBUG_PRINT(i);
    // DEBUG_PRINT(": ");
    // DEBUG_PRINTLN(adc[i]);
  // }

  // adcRegInMin    = min(adc[0], adcRegInMin);
  // adcRegOutMin   = min(adc[1], adcRegOutMin); 
  // adcAcuMin      = min(adc[2], adcAcuMin);
  // adc12VMin      = min(adc[3], adc12VMin);
  // adcRegInMax    = max(adc[0], adcRegInMax);
  // adcRegOutMax   = max(adc[1], adcRegOutMax); 
  // adcAcuMax      = max(adc[2], adcAcuMax);
  // adc12VMax      = max(adc[3], adc12VMax);
  adc = ads.readADC_Differential_0_1();
  adcRegOutMin   = min(adc, adcRegOutMin); 
  adcRegOutMax   = max(adc, adcRegOutMax); 

  return true;
}

void readCurrent() {
  DEBUG_PRINT("Point ");
  DEBUG_PRINT(analogRead(VIN));
  DEBUG_PRINT("  ");
  float voltage_raw = (3.3 / 1023.0) * (analogRead(VIN));// Read the voltage from sensor
  //float voltage_raw = analogRead(VIN);// Read the voltage from sensor
  DEBUG_PRINT(" RAW voltage ");
  DEBUG_PRINT(voltage_raw);
  voltage =  voltage_raw - 2.57; // 0.007 is a value to make voltage zero when there is no current

  currentIN = voltage / FACTOR;
  if(fabs(voltage) <= cutOff ) {  //< 0.04
    currentIN = 0;
    DEBUG_PRINT(" zero current ");
  }
  DEBUG_PRINT(" V: ");
  DEBUG_PRINT2(voltage,3);// print voltage with 3 decimal places
  DEBUG_PRINT("V, I: ");
  DEBUG_PRINT2(currentIN,2); // print the current with 2 decimal places
  DEBUG_PRINTLN("A");


  napeti = (analogRead(CHAROUT) * 5000.0) / 1023.0;
  proud = (napeti - offset) / konstanta;



}

bool sendDataHA(void *) {
  readCurrent();
  
  DEBUG_PRINT("Charger input :");
  DEBUG_PRINTLN(digitalRead(CHARIN));
  
  digitalWrite(BUILTIN_LED, LOW);
  DEBUG_PRINTLN(F(" - I am sending data to HA"));
  
//Adafruit_MQTT_Subscribe restart                = Adafruit_MQTT_Subscribe(&mqtt, MQTTBASE "restart");
  SenderClass sender;

  sender.add("voltage", voltage);
  sender.add("voltage_raw", voltage_raw);
 
  sender.add("currentIN", currentIN);
  
  //stav vystupu
  sender.add("chargerOUT", digitalRead(CHARIN));

  //napeti na regulatoru
  sender.add("adcRegInMin", adcRegInMin);
  sender.add("adcRegOutMin", adcRegOutMin);
  sender.add("adcAcuMin", adcAcuMin);
  sender.add("adc12VMin", adc12VMin);
  sender.add("adcRegInMax", adcRegInMax);
  sender.add("adcRegOutMax", adcRegOutMax);
  sender.add("adcAcuMax", adcAcuMax);
  sender.add("adc12VMax", adc12VMax);
  
  adcRegInMin   = MAX;
  adcRegOutMin  = MAX;
  adcAcuMin     = MAX;   
  adc12VMin     = MAX;   
  adcRegInMax   = 0; 
  adcRegOutMax  = 0;
  adcAcuMax     = 0;   
  adc12VMax     = 0;   
  
  
  
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
  sender.add("HeartBeat", heartBeat++);
  sender.add("RSSI", WiFi.RSSI());
  
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}
