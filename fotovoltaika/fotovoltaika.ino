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

#include "Configuration.h"

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#include "Sender.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>
//#include <Adafruit_INA219.h>
#include <FS.h>          //this needs to be first


#define time
#ifdef time
#include <TimeLib.h>
#include <Timezone.h>
WiFiUDP EthernetUdp;
static const char     ntpServerName[]       = "tik.cesnet.cz";
//const int timeZone = 2;     // Central European Time
//Central European Time (Frankfurt, Paris)
TimeChangeRule        CEST                  = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule        CET                   = {"CET",  Last, Sun, Oct, 3, 60 };     //Central European Standard Time
Timezone CE(CEST, CET);
unsigned int          localPort             = 8888;  // local port to listen for UDP packets
time_t getNtpTime();
#define TIMEX   0
#define TIMEY   3
#endif

//voltage and current meassurement
Adafruit_ADS1115 ads1(0x48);  //mereni proudu ADDR to GND - pravý
Adafruit_ADS1115 ads2(0x49);  //mereni napeti ADDR to VCC - levý
//Adafruit_ADS1115 ads3(0x4B);  //mereni napeti ADDR to SCL


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

//for LED status
Ticker ticker;

bool isDebugEnabled()
{
#ifdef verbose
  return true;
#endif // verbose
  return false;
}


float   relayONVoltageBig           = 13.5;
float   relayONVoltageSmall         = 12.5;
float   relayOFFVoltage             = 11.f;


byte relayStatus                    = RELAY_OFF;
byte relayMode                      = RELAY_MODE_AUTO;
  
int duvodZmenyStavuRele             = 0;
//ADC_MODE(ADC_VCC);

uint32_t heartBeat                  = 0;

//mereni napeti   
float      voltageRegInMin          = MAX; //vystup z panelu, rozsah 0-20V
float      voltageRegIn             = 12.f; 
float      voltageRegOutMin         = MAX; //vystup z regulatoru, rozsah 0-15V
float      voltageRegInMax          = MIN; //vystup z panelu, rozsah 0-20V
float      voltageRegOutMax         = MIN; //vystup z regulatoru, rozsah 0-15V
float      voltageRegOut            = 12.f; 
float      voltageSupply            = MIN;
float      forecastedEnergyTomorrow = -1.f;  
uint32_t  lastReadADC               = 0;              //interval mezi ctenim sensoru

//mereni proudu
float     currentRegIn              = 0.f;   //Ampers
float     currentRegOut             = 0.f;
      
float     currentRegInSum           = 0.f;
float     currentRegOutSum          = 0.f;
int32_t   intervalMSec              = 0;

long      runSecToday               = 0;
float     AhPanelToday              = 0;
float     AhOutToday                = 0;

bool      todayClear                = false;
bool      dispClear                 = false;

float     lastRelayOff              = 0;


//Adafruit_INA219 ina219_2; //intput
//Adafruit_INA219 ina219_1(0x41); //output

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
  digitalWrite(STATUS_LED, !state);          // set pin to the opposite state
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
  
  if (strcmp(topic, (String(mqtt_base) + "/cmnd/" + String(mqtt_topic_relay)).c_str())==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("set manual control relay to ");
    relayMode = val.toInt();
    if (val.toInt()==1) {
      DEBUG_PRINTLN(F("ON"));
    } else if (val.toInt()==0) {
      DEBUG_PRINTLN(F("OFF"));
    } else {
      DEBUG_PRINTLN(F("AUTO"));
    }
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_restart)).c_str())==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("RESTART");
    ESP.restart();
  } else if (strcmp(topic, mqtt_topic_weather)==0) {
    DEBUG_PRINT("Temperature from Meteo: ");
    DEBUG_PRINTLN(val.toFloat());
    //displayValue(TEMPERATURE_X,TEMPERATURE_Y, (int)round(val.toFloat()), 3, 0);
    //lcd.write(byte(0));
    //lcd.print("C");
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_relayOFFVoltage)).c_str())==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("set OFF voltage to ");
    relayOFFVoltage = val.toFloat();
    saveConfig();
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_relayONVoltageBig)).c_str())==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("set ON big voltage to ");
    relayONVoltageBig = val.toFloat();
    saveConfig();
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_forecast)).c_str())==0) {
    DEBUG_PRINT("Forecast from solcast ");
    lcd.setCursor(FORECAST_X-3,FORECAST_Y);
    lcd.print("FP:");
    displayValue(FORECAST_X,FORECAST_Y, val.toFloat(), 2, 1);
    forecastedEnergyTomorrow = val.toFloat();
    lcd.print("kWh");
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_relayONVoltageSmall)).c_str())==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("set ON small voltage to ");
    relayONVoltageSmall = val.toFloat();
    saveConfig();
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
  pinMode(STATUS_LED, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  digitalWrite(STATUS_LED, HIGH); //nesviti
  digitalWrite(LED2PIN, HIGH);
  pinMode(RELAY1PIN, OUTPUT);
  pinMode(RELAY2PIN, OUTPUT);
  digitalWrite(RELAY1PIN, relayStatus);
  digitalWrite(RELAY2PIN, relayStatus);
  pinMode(PIRPIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(PIRPIN), PIREvent, CHANGE);

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

  WiFi.printDiag(Serial);
    
  bool validConf = readConfig();
  if (!validConf) {
    DEBUG_PRINTLN(F("ERROR config corrupted"));
  }
  
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
  ArduinoOTA.setHostname(HOSTNAMEOTA);
  ArduinoOTA.onStart([]() {
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
  //ads3.setGain(GAIN_TWOTHIRDS);        // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);            // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);            // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);           // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);          // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);        // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads1.begin();
  ads2.begin();
  //ads3.begin();
  
    // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  //ina219_1.begin();
  //ina219_2.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219_1.setCalibration_16V_400mA();
  //ina219_2.setCalibration_32V_2A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  
  //setup timers
  timer.every(SEND_DELAY, sendDataHA);
  timer.every(SENDSTAT_DELAY, sendStatisticHA);
  timer.every(READADC_DELAY, readADC);
  timer.every(1000, calcStat);
#ifdef time  
  timer.every(500, displayTime);
#endif
  void * a;
  sendStatisticHA(a);

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(STATUS_LED, HIGH);
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
 
  nulStat();
  displayClear();
 
}

//---------------------------------------------R E L A Y ------------------------------------------------
void relay() {
  if (relayMode==RELAY_MODE_AUTO) {
    if (millis() - lastRelayOff > RELAYDELAYOFFON) {
      //-----------------------------------zmena 0-1--------------------------------------------
      if (relayStatus == RELAY_OFF) {
        bool zapni=false;
        if (voltageRegOut > relayONVoltageBig) { //voltageRegOut > 13.5
          duvodZmenyStavuRele=1;
          zapni = true;
        } else if (currentRegIn > CURRENT4ONBIG) { //currentRegIn > 6
          duvodZmenyStavuRele=2;
          zapni = true;
        } else if (currentRegIn > CURRENT4ONSMALL && voltageRegOut >= relayONVoltageSmall) { //currentRegIn > 4) && voltageRegOut >= 12.5
          duvodZmenyStavuRele=3;
          zapni = true;
        }
        if (zapni) {
          relayStatus = RELAY_ON;
          changeRelay(relayStatus);
          sendRelayHA(1);
        }
      //-----------------------------------zmena 1-0--------------------------------------------
      } else if (relayStatus == RELAY_ON) {
        bool vypni=false;
        if (voltageRegOut <= relayOFFVoltage) { //voltageRegOut <= 11.8
          duvodZmenyStavuRele=4;
          vypni = true;
        } else if (forecastedEnergyTomorrow < FORECASTED_LIMIT_OFF && currentRegIn < CURRENT4ONSMALL && voltageRegOut < relayONVoltageSmall) { //forecastedEnergyTomorrow < 0.06 && currentRegIn < 2 && voltageRegOut < 12.5
          duvodZmenyStavuRele=5;
          vypni = true;
        } else if (currentRegIn < currentRegOut && voltageRegOut < relayONVoltageSmall) { //currentRegIn < currentRegOut && voltageRegOut < 12.5
          duvodZmenyStavuRele=6;
          vypni = true;
        }
        if (vypni) {
          relayStatus = RELAY_OFF;
          changeRelay(relayStatus);
          sendRelayHA(0);
          lastRelayOff = millis();
        }
      }
      if (duvodZmenyStavuRele==-1) {
        duvodZmenyStavuRele=0;
      }
    } else {
      duvodZmenyStavuRele=-1;
    }
  } else if (relayStatus == RELAY_OFF && relayMode==RELAY_MODE_ON) {
      relayStatus = RELAY_ON;
      changeRelay(relayStatus);
      duvodZmenyStavuRele=6;
      sendRelayHA(1);
  } else if (relayStatus == RELAY_ON && relayMode==RELAY_MODE_OFF) {
      relayStatus = RELAY_OFF;
      changeRelay(relayStatus);
      duvodZmenyStavuRele=7;
      sendRelayHA(1);
  }
  dispRelayStatus();
}


void nulStat() {
 //nulovani statistik o pulnoci
  if (hour()==0 && !todayClear) {
    todayClear =true;
    runSecToday = 0;
    AhPanelToday = 0;
  } else if (hour()>0) {
    todayClear = false;
  }
}

void displayClear() {
  if (minute()==0 && second()==0) {
    if (!dispClear) { 
      lcd.clear();
      dispClear = true;
    }
  } else {
    dispClear = false;
  }
}

bool readADC(void *) {

  int32_t dilkuSupply            = ads1.readADC_SingleEnded(CHANNEL_VOLTAGE_SUPPLY);
  int32_t dilkuInputCurrent      = ads1.readADC_SingleEnded(CHANNEL_REG_IN_CURRENT);
  int32_t dilkuOutputCurrent     = ads1.readADC_SingleEnded(CHANNEL_REG_OUT_CURRENT);
  // DEBUG_PRINT("dilkuSupply:");
  // DEBUG_PRINT(dilkuSupply);
  // DEBUG_PRINT(", dilkuInputCurrent:");
  // DEBUG_PRINT(dilkuInputCurrent);
  // DEBUG_PRINT(", dilkuOutputCurrent:");
  // DEBUG_PRINTLN(dilkuOutputCurrent);
  
  voltageSupply    = ((float)dilkuSupply * MVOLTDILEKADC1); //in mV  example 26149 * 0.1875 = 4902,938mV
  // DEBUG_PRINT("voltageSupply:");
  // DEBUG_PRINTLN(voltageSupply);

  int32_t dilkuInputVoltage      = ads2.readADC_Differential_0_1();  //solar panel
  int32_t dilkuOutputVoltage     = ads2.readADC_Differential_2_3();  //output
  // DEBUG_PRINT("dilkuInputVoltage:");
  // DEBUG_PRINT(dilkuInputVoltage);
  // DEBUG_PRINT(", dilkuOutputVoltage:");
  // DEBUG_PRINT(dilkuOutputVoltage);
  
  float loadvoltage = (dilkuOutputVoltage * MVOLTDILEKADC2 * KOEF_OUTPUT_VOLTAGE) / V2MV; // 13.0f;
  
  if (loadvoltage > MIN && loadvoltage < MAX) {
    voltageRegOut = loadvoltage;
    voltageRegOutMin   = min(loadvoltage, voltageRegOutMin);
    voltageRegOutMax   = max(loadvoltage, voltageRegOutMax);
    // DEBUG_PRINTLN(voltageRegOutMin);
    // DEBUG_PRINTLN(voltageRegOutMax);
  }

  loadvoltage = dilkuOutputVoltage * MVOLTDILEKADC2 * KOEF_INPUT_VOLTAGE; // 13.0f;
  if (loadvoltage > MIN && loadvoltage < MAX) {
    voltageRegIn = 12.f; //loadvoltage;
    voltageRegInMin    = 12.f; //min(loadvoltage, voltageRegIMin);
    voltageRegInMax    = 12.f; //max(loadvoltage, voltageRegInMax);
  }
  
  if (lastReadADC==0) {
    lastReadADC = millis();
  }
  
  uint32_t diff = millis()-lastReadADC;
  
  currentRegIn  = ((float)(dilkuInputCurrent * 2 - dilkuSupply) * MVOLTDILEKADC1) / MVAMPERIN / 2; //in Amp example ((15170 * 2 - 25852) * 0.1875) / 40 = ((30340 - 25852) * 0.1875) / 40 = 4488 * 0.1875 / 40
  currentRegInSum += currentRegIn * diff;
  // DEBUG_PRINT("currentRegIn:");
  // DEBUG_PRINTLN(currentRegIn);
  
  currentRegOut = ((float)(dilkuOutputCurrent * 2 - dilkuSupply)   * MVOLTDILEKADC1) / MVAMPEROUT / 2;
  currentRegOutSum += currentRegOut * diff;
  // DEBUG_PRINT("currentRegOut:");
  // DEBUG_PRINTLN(currentRegOut);
  
  intervalMSec += diff;
 
  lastReadADC = millis();
  return true;
}

bool sendDataHA(void *) {
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  SenderClass sender;

  sender.add("status/" + String(mqtt_topic_relay), relayStatus); //ON, OFF
  
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
  if (relayStatus==RELAY_ON) {
    sender.add("currentRegOut",   currentRegOutSum  / (float)intervalMSec);
    sender.add("powerOut",        (currentRegOutSum / (float)intervalMSec) * voltageRegOutMax);
  } else {
    sender.add("currentRegOut",   0);
    sender.add("powerOut",        0);
  }
  sender.add("intervalSec",       (float)intervalMSec/1000.f);
  
  sender.add("NapetiSbernice",    voltageSupply);

  voltageRegInMin   = MAX;
  voltageRegOutMin  = MAX;
  voltageRegInMax   = MIN; 
  voltageRegOutMax  = MIN;
  intervalMSec      = 0;
  currentRegInSum   = 0.f;
  currentRegOutSum  = 0.f;
  
  DEBUG_PRINTLN(F("Calling MQTT"));

  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}

bool sendStatisticHA(void *) {
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  //printSystemTime();
  DEBUG_PRINTLN(F(" - I am sending statistic to HA"));
  DEBUG_PRINTLN(F("Calling MQTT"));

  SenderClass sender;
  sender.add("VersionSWFotovoltaika", VERSION);
  sender.add("HeartBeat", heartBeat++);
  if (heartBeat % 10 == 0) sender.add("RSSI", WiFi.RSSI());
  
  sender.add("relayOFFVoltage", relayOFFVoltage);
  sender.add("relayONVoltageBig", relayONVoltageBig);
  sender.add("relayONVoltageSmall", relayONVoltageSmall);
  sender.add("duvodZmenyStavuRele", duvodZmenyStavuRele);

  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);

  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}


void sendRelayHA(byte akce) {
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  SenderClass sender;
  sender.add("relayChange", akce);
  sender.add("duvodZmenyStavuRele", duvodZmenyStavuRele);
  sender.add("status/" + String(mqtt_topic_relay), relayStatus); //ON, OFF
  
 
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---------------------------------------------D I S P L A Y ------------------------------------------------
void lcdShow() {
  if (display==DISPLAY_MAIN) {
    //01234567890123456789     //Reg In    Acu    Out  
    //P 254W  100W    50Ah     //power
    //I10.8A 10.1A    25Ah     //current
    //U12.6V 5.45V    120m     //voltage
    //15:23 -10°C      OFF     //hour  temperature  relay status 
    lcd.setCursor(0,0);
    lcd.print("P");
    //displayValue(POZREGIN_POWERX,POZREGIN_POWERXY, voltageRegInMax*currentRegIn, false);
    displayValue(POZREGIN_POWERX,POZREGIN_POWERXY, voltageRegIn*currentRegIn, 3, 0);
    lcd.print(POWER_UNIT);
    //displayValue(POZREGOUT_POWERX,POZREGOUT_POWERXY, voltageRegOutMax*currentRegOut, false);
    displayValue(POZREGOUT_POWERX,POZREGOUT_POWERXY, voltageRegOut*currentRegOut, 3, 0);
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
    displayValue(POZREGIN_VOLTAGEX,POZREGIN_VOLTAGEY, voltageRegIn, 2, 1);
    lcd.print(VOLTAGE_UNIT);
    //displayValue(POZREGOUT_VOLTAGEX,POZREGOUT_VOLTAGEY, voltageRegOutMax, true);
    displayValue(POZREGOUT_VOLTAGEX,POZREGOUT_VOLTAGEY, voltageRegOut, 2, 1);
    lcd.print(VOLTAGE_UNIT);

    lcd.setCursor(SYSTEM_VOLTAGEX, SYSTEM_VOLTAGEY);
    lcd.print(voltageSupply/V2MV, 1);
    lcd.print(VOLTAGE_UNIT);

    displayValue(RUNMINTODAY_X,RUNMINTODAY_Y, runSecToday / 60, 4, 0);  //runMsToday in s -> min
    lcd.print(MIN_UNIT);

    displayValue(AHPANELTODAY_X,AHPANELTODAY_Y, AhPanelToday / 3600, 3, 0);  //AhPanelToday in Ws -> Wh
    lcd.print(AH_UNIT);

    
  }
}

void displayValue(int x, int y, float value, byte cela, byte des) {
  char buffer [18];
  if (des==0) {
    value = round(value);
  }
 
  char format[5];
  char cislo[2];
  itoa(cela, cislo, 10);
  strcpy(format, "%");
  strcat(format, cislo);
  strcat(format, "d");

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
      client.subscribe((String(mqtt_base) + "/cmnd/" + String(mqtt_topic_relay)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_restart)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_relayONVoltageBig)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_relayONVoltageSmall)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_relayOFFVoltage)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_forecast)).c_str());
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
  if (relayMode==RELAY_ON) {
    lcd.print("MON");
    digitalWrite(LED2PIN, LOW);
  } else if (relayMode==RELAY_OFF) {
    lcd.print("MOF");
    digitalWrite(LED2PIN, HIGH);
  } else if (relayStatus==RELAY_ON) {
    lcd.print(" ON");
    digitalWrite(LED2PIN, LOW);
  } else if (relayStatus==RELAY_OFF) {
    lcd.print("OFF");
    digitalWrite(LED2PIN, HIGH);
  }
}


void changeRelay(byte status) {
  // DEBUG_PRINT("changeRelay:");
  // DEBUG_PRINT(status);
  // DEBUG_PRINT(" voltageRegOutMax:");
  // DEBUG_PRINT(voltageRegOutMax);
  // DEBUG_PRINT(" relayOFFVoltage:");
  // DEBUG_PRINTLN(relayOFFVoltage);
  digitalWrite(RELAY2PIN, status);
  if (status==RELAY_ON) {
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
  showDoubleDot = !showDoubleDot;
  lcdShow();
  return true;
}
#endif


bool calcStat(void *) {  //run each second from timer
  if (relayStatus == RELAY_ON) {
    runSecToday ++;
    AhPanelToday += currentRegOut; //Ws
  }
  return true;
}

bool readConfig() {
  DEBUG_PRINT(F("Mounting FS..."));

  if (SPIFFS.begin()) {
    DEBUG_PRINTLN(F(" mounted!"));
    if (SPIFFS.exists(CFGFILE)) {
      // file exists, reading and loading
      DEBUG_PRINTLN(F("Reading config file"));
      File configFile = SPIFFS.open(CFGFILE, "r");
      if (configFile) {
        DEBUG_PRINTLN(F("Opened config file"));

        char json[500];
        while (configFile.available()) {
         int l = configFile.readBytesUntil('\n', json, sizeof(json));
         json[l] = 0;
         DEBUG_PRINTLN(json);
        }

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, json);
        relayONVoltageBig = doc["relayONVoltageBig"];
        DEBUG_PRINT(F("relayONVoltageBig: "));
        DEBUG_PRINTLN(relayONVoltageBig);
        relayONVoltageSmall = doc["relayONVoltageSmall"];
        DEBUG_PRINT(F("relayONVoltageSmall: "));
        DEBUG_PRINTLN(relayONVoltageSmall);
        relayOFFVoltage = doc["relayOFFVoltage"];
        DEBUG_PRINT(F("relayOFFVoltage: "));
        DEBUG_PRINTLN(relayOFFVoltage);
        
       
        return true;
      }
      DEBUG_PRINTLN(F("ERROR: unable to open config file"));
    } else {
      DEBUG_PRINTLN(F("ERROR: config file not exist"));
    }
  } else {
    DEBUG_PRINTLN(F(" ERROR: failed to mount FS!"));
  }
  return false;
}

bool saveConfig() {
  DEBUG_PRINTLN(F("Saving config..."));

  // if SPIFFS is not usable
  if (!SPIFFS.begin() || !SPIFFS.exists(CFGFILE) ||
      !SPIFFS.open(CFGFILE, "w"))
  {
    DEBUG_PRINTLN(F("Need to format SPIFFS: "));
    SPIFFS.end();
    SPIFFS.begin();
    DEBUG_PRINTLN(SPIFFS.format());
  }

  StaticJsonDocument<1024> doc;

  doc["relayONVoltageBig"]         = relayONVoltageBig;
  doc["relayONVoltageSmall"]       = relayONVoltageSmall;
  doc["relayOFFVoltage"]           = relayOFFVoltage;

  lcd.clear();
 
  File configFile = SPIFFS.open(CFGFILE, "w+");
  if (!configFile) {
    DEBUG_PRINTLN(F("Failed to open config file for writing"));
    SPIFFS.end();
    lcd.print("Failed to open config file for writing");
    delay(2000);
    lcd.clear();
    return false;
  } else {
    if (isDebugEnabled) {
      serializeJson(doc, Serial);
    }
    serializeJson(doc, configFile);
    //json.printTo(configFile);
    configFile.close();
    SPIFFS.end();
    DEBUG_PRINTLN(F("\nSaved successfully"));
    lcd.print("Config saved.");
    delay(2000);
    lcd.clear();
    return true;
  }
}
