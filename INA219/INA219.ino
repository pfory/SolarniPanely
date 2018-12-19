#include <Wire.h>
#include <Adafruit_INA219.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <FS.h>          //this needs to be first
#include <Ticker.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include "DoubleResetDetector.h" // https://github.com/datacute/DoubleResetDetector
#include "Sender.h"
#include <Wire.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ota
#ifdef ota
#include <ArduinoOTA.h>
#define HOSTNAMEOTA   "SolarMereni"
#endif

//#define serverHTTP
#ifdef serverHTTP
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#endif

#define verbose
#ifdef verbose
 #define PORTSPEED 115200
 #define DEBUG_PRINT(x)         Serial.print (x)
 #define DEBUG_PRINTDEC(x)      Serial.print (x, DEC)
 #define DEBUG_PRINTLN(x)       Serial.println (x)
 #define DEBUG_PRINTF(x, y)     Serial.printf (x, y)
 #define DEBUG_PRINTHEX(x)      Serial.print (x, HEX)
 #define SERIAL_BEGIN           Serial.begin(PORTSPEED)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTDEC(x)
 #define DEBUG_PRINTLN(x)
 #define DEBUG_PRINTF(x, y)
 #define SERIAL_BEGIN
#endif 

char                  mqtt_server[40]       = "192.168.1.56";
uint16_t              mqtt_port             = 1883;
char                  mqtt_username[40]     = "datel";
char                  mqtt_key[20]          = "hanka12";
char                  mqtt_base[60]         = "/home/SolarMereni";
char                  static_ip[16]         = "192.168.1.116";
char                  static_gw[16]         = "192.168.1.1";
char                  static_sn[16]         = "255.255.255.0";

#define DRD_TIMEOUT       1
// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS       0
DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

#define CFGFILE "/config.json"

const unsigned long   sendDelay             = 5000; //in ms
const unsigned long   sendStatDelay         = 60000;
                
float                 shuntvoltage           = 0;
float                 busvoltage             = 0;
float                 current_mA             = 0;
float                 loadvoltage            = 0;
float                 power_mW               = 0;


float versionSW                             = 0.1;
char versionSWString[]                      = "Solar mereni v"; //SW name & version
uint32_t heartBeat                          = 0;


Adafruit_INA219 ina219;

bool isDebugEnabled() {
#ifdef verbose
  return true;
#endif // verbose
  return false;
}

//for LED status
#include <Ticker.h>
Ticker ticker;


#include <timer.h>
auto timer = timer_create_default(); // create a timer with default settings
Timer<> default_timer; // save as above

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}
  
//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  DEBUG_PRINTLN("Should save config");
  shouldSaveConfig = true;
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  DEBUG_PRINTLN("Entered config mode");
  DEBUG_PRINTLN(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  DEBUG_PRINTLN(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}


void setup(void) {
  SERIAL_BEGIN;
  DEBUG_PRINT(versionSWString);
  DEBUG_PRINTLN(versionSW);
 
  pinMode(BUILTIN_LED, OUTPUT);
  ticker.attach(1, tick);
  
  setupWifi();
  
  setupOTA();

  uint32_t currentFrequency;
    
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  //setup timers
  timer.every(sendDelay, sendDataHA);
  timer.every(sendStatDelay, sendStatisticHA);

  DEBUG_PRINTLN(" Ready");
 
  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);

}

void loop(void) {
  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif

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

  delay(2000);
}

void setupWifi() {
  DEBUG_PRINTLN("-------WIFI Setup---------");
  WiFi.printDiag(Serial);
    
  //WiFiManager
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  
  wifiManager.setTimeout(30);
  wifiManager.setConnectTimeout(10); 
  wifiManager.setConfigPortalTimeout(60);
  
  if (!wifiManager.autoConnect("Clock", "password")) { 
    DEBUG_PRINTLN("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
  
  //if you get here you have connected to the WiFi
  DEBUG_PRINTLN("CONNECTED");
  DEBUG_PRINT("Local ip : ");
  DEBUG_PRINTLN(WiFi.localIP());
  DEBUG_PRINTLN(WiFi.subnetMask());

}

void setupOTA() {
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
}

bool sendDataHA(void *) {
  digitalWrite(BUILTIN_LED, LOW);
  DEBUG_PRINTLN(F(" - I am sending data to HA"));
  
//Adafruit_MQTT_Subscribe restart                = Adafruit_MQTT_Subscribe(&mqtt, MQTTBASE "restart");
  SenderClass sender;
 
  sender.add("busVoltage", busvoltage);
  sender.add("shuntVoltage", shuntvoltage);
  sender.add("loadVoltage", loadvoltage);
  sender.add("current", current_mA);
  sender.add("power", power_mW);
  
  DEBUG_PRINTLN(F("Calling MQTT"));

  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}

bool sendStatisticHA(void *) {
  digitalWrite(BUILTIN_LED, LOW);
  DEBUG_PRINTLN(F(" - I am sending statistic to HA"));

  SenderClass sender;
  sender.add("VersionSW", versionSW);
  //sender.add("Napeti",  ESP.getVcc());
  sender.add("HeartBeat", heartBeat++);
  sender.add("RSSI", WiFi.RSSI());
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}

