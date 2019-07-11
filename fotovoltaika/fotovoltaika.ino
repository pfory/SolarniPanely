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
  
*/

const float VCC = 5.0;// supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_INA219.h>

Adafruit_ADS1115 ads1(0x48);  //ADDR to GND
Adafruit_ADS1115 ads2(0x49);  //ADDR to 5V

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
#define     VERSION                          "0.21"
#define     SW_NAME                          "Fotovoltaika"

#define SEND_DELAY                           5000  //prodleva mezi poslanim dat v ms
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
char                  mqtt_base[60]         = "/home/SolarMereni";
char                  static_ip[16]         = "192.168.1.116";
char                  static_gw[16]         = "192.168.1.1";
char                  static_sn[16]         = "255.255.255.0";

#define VIN                                  A0 // define the Arduino pin A0 as voltage input (V in)
#define LED2PIN                              D3 //stav rele
#define LED1PIN                              D4 //
#define CHAROUT                              D7 //vystup z regulatoru
#define RELAYPIN                             D8 //pin rele
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

Adafruit_INA219 ina219;

float                 shuntvoltage           = 0;
float                 busvoltage             = 0;
float                 current_mA             = 0;
float                 loadvoltage            = 0;
float                 power_mW               = 0;

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
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  pinMode(CHAROUT, INPUT);
  pinMode(RELAYPIN, OUTPUT);

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
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  
  //setup timers
  timer.every(SEND_DELAY, sendDataHA);
  timer.every(SENDSTAT_DELAY, sendStatisticHA);
  timer.every(READADC_DELAY, readADC);
  sendStatisticHA;

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(LED1PIN, HIGH);

}

void loop() {
  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif
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
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
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
  
  sender.add("busVoltage", busvoltage);
  sender.add("shuntVoltage", shuntvoltage);
  sender.add("loadVoltage", loadvoltage);
  sender.add("current", current_mA);
  sender.add("power", power_mW);
  
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
