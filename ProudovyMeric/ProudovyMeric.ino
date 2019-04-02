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

#define AUTOCONNECTNAME   "MereniProudu"
#define AUTOCONNECTPWD    "password"

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
  Serial.println("ACS758 Current Sensor");
  Serial.print("cutOff ");
  Serial.println(cutOff);
  Serial.print("QOV ");
  Serial.println(QOV);
  Serial.print("Factor ");
  Serial.println(FACTOR);
  
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
}


bool sendDataHA(void *) {
  Serial.print("Point ");
  Serial.print(analogRead(VIN));
  Serial.print("  ");
  float voltage_raw = (3.3 / 1024.0) * (analogRead(VIN) - 59);// Read the voltage from sensor
  //float voltage_raw = analogRead(VIN);// Read the voltage from sensor
  Serial.print(" RAW voltage ");
  Serial.print(voltage_raw);
  voltage =  voltage_raw - QOV + 0.007 ;// 0.007 is a value to make voltage zero when there is no current
  Serial.print(" voltage ");
  Serial.print(fabs(voltage),3);
  float current = voltage / FACTOR;
  if(fabs(voltage) <= cutOff ) {  //> 0.04
    current = 0;
    Serial.print(" zero current ");
  }
  Serial.print(" V: ");
  Serial.print(voltage,3);// print voltage with 3 decimal places
  Serial.print("V, I: ");
  Serial.print(current,2); // print the current with 2 decimal places
  Serial.println("A");
  
  Serial.print("Charger output :");
  Serial.println(digitalRead(CHAROUT));
  
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