#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.0;// supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)

char                  mqtt_server[40]       = "192.168.1.56";
uint16_t              mqtt_port             = 1883;
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#include "Sender.h"

//for LED status
#include <Ticker.h>
Ticker ticker;

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
#define CHAROUT                              D7


float cutOffLimit = 10.0;// reading cutt off current. 1.00 is 1 Amper

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
const float QOV =   quiescent_Output_voltage [model] * VCC;// set quiescent Output voltage for selected model 0.5*5 = 2.5
float voltage;// internal variable for voltage
float cutOff = FACTOR/cutOffLimit;// convert current cut off to mV 0.04

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
  //Robojax.com ACS758 Current Sensor 
  Serial.begin(115200);// initialize serial monitor
  DEBUG_PRINTLN("ACS758 Current Sensor");
  DEBUG_PRINT("cutOff ");
  DEBUG_PRINTLN(cutOff);
  DEBUG_PRINT("QOV ");
  DEBUG_PRINTLN(QOV);
  DEBUG_PRINT("Factor ");
  DEBUG_PRINTLN(FACTOR);
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(CHAROUT, INPUT);

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
  
  //setup timers
  timer.every(30000, sendDataHA);
  //timer.every(sendStatDelay, sendStatisticHA);
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


bool sendDataHA(void *) {
  DEBUG_PRINT("Point ");
  DEBUG_PRINT(analogRead(VIN));
  DEBUG_PRINT("  ");
  float voltage_raw = (3.3 / 1024.0) * (analogRead(VIN));// Read the voltage from sensor
  //float voltage_raw = analogRead(VIN);// Read the voltage from sensor
  DEBUG_PRINT(" RAW voltage ");
  DEBUG_PRINT(voltage_raw);
  voltage =  voltage_raw - QOV - 0.184 ;// 0.007 is a value to make voltage zero when there is no current

  float current = voltage / FACTOR;
  if(fabs(voltage) <= cutOff ) {  //> 0.04
    current = 0;
    DEBUG_PRINT(" zero current ");
  }
  DEBUG_PRINT(" V: ");
  DEBUG_PRINT2(voltage,3);// print voltage with 3 decimal places
  DEBUG_PRINT("V, I: ");
  DEBUG_PRINT2(current,2); // print the current with 2 decimal places
  DEBUG_PRINTLN("A");
  
  DEBUG_PRINT("Charger output :");
  DEBUG_PRINTLN(digitalRead(CHAROUT));
  
  digitalWrite(BUILTIN_LED, LOW);
  DEBUG_PRINTLN(F(" - I am sending data to HA"));
  
//Adafruit_MQTT_Subscribe restart                = Adafruit_MQTT_Subscribe(&mqtt, MQTTBASE "restart");
  SenderClass sender;
 
  sender.add("current", current);
  sender.add("chargerOUT", digitalRead(CHAROUT));
  
  DEBUG_PRINTLN(F("Calling MQTT"));

  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}