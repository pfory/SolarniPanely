/*
--------------------------------------------------------------------------------------------------------------------------
POWERMETER - shows values from FV
Petr Fory pfory@seznam.cz
GIT - https://github.com/pfory/
*/

#include "Configuration.h"


#ifdef time
WiFiUDP EthernetUdp;
static const char     ntpServerName[]       = "tik.cesnet.cz";
TimeChangeRule        CEST                  = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule        CET                   = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);
unsigned int          localPort             = 8888;  // local port to listen for UDP packets
time_t getNtpTime();
#endif

uint32_t              heartBeat                   = 0;
bool                  dispClear                   = false;


LiquidCrystal_I2C lcd(LCDADDRESS,LCDCOLS,LCDROWS);  // set the LCD
#define PRINT_SPACE  lcd.print(" ");
volatile bool showDoubleDot                 = false;
//#define DISPLAY_MAIN                         0

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

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

ADC_MODE(ADC_VCC); //vcc read

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS]                 = {
                                            {'1','2','3','A'},
                                            {'4','5','6','B'},
                                            {'7','8','9','C'},
                                            {'*','0','#','D'}
};
byte rowPins[ROWS]                    = {7,6,5,4}; //connect to the row pinouts of the keypad
byte colPins[COLS]                    = {3,2,1,0}; //connect to the column pinouts of the keypad

//for LED status
Ticker ticker;

auto timer = timer_create_default(); // create a timer with default settings
Timer<> default_timer; // save as above


void tick() {
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
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

//MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  // char * pEnd;
  // long int valL;
  String val =  String();
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  for (int i=0;i<length;i++) {
    DEBUG_PRINT((char)payload[i]);
    val += (char)payload[i];
  }
  DEBUG_PRINTLN();
  
  if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_restart)).c_str())==0) {
    printMessageToLCD(topic, val);
    DEBUG_PRINT("RESTART");
    ESP.restart();
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_netinfo)).c_str())==0) {
    printMessageToLCD(topic, val);
    sendNetInfoMQTT();
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/powerFVToday").c_str())==0) {
    lcd.setCursor(0,2);
    lcd.print(zaokrouhli(val.toFloat(), 3.f),3);
    lcd.print("kWh");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/pvEstimateToday").c_str())==0) {
    lcd.setCursor(13,1);
    lcd.print(zaokrouhli(val.toFloat(), 2.f),2);
    lcd.print("kWh");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/pvEstimateTomorow").c_str())==0) {
    lcd.setCursor(13,2);
    lcd.print(zaokrouhli(val.toFloat(), 2.f),2);
    lcd.print("kWh");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/utiPower").c_str())==0) {
    lcd.setCursor(0,3);
    lcd.print(zaokrouhli(val.toFloat(), 0.f),0);
    lcd.print("W");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/utiToday").c_str())==0) {
    lcd.setCursor(6,3);
    lcd.print(zaokrouhli(val.toFloat(), 1.f),1);
    lcd.print("kWh");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/battcappa").c_str())==0) {
    lcd.setCursor(16,3);
    lcd.print(zaokrouhli(val.toFloat(), 0.f),0);
    lcd.print("%");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/pvchargew").c_str())==0) {
    lcd.setCursor(0,1);
    val.replace("0"," ");
    if (val=="     ") {
      val="    0";
    }
    lcd.print(val);
    lcd.print("W");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/masterstatus").c_str())==0) {
    lcd.setCursor(0,0);
    if (val=="Line") {
      lcd.print("LINE");
    } else if (val=="Battery") {
      lcd.print("BATT");
    }
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/charge").c_str())==0) {
    lcd.setCursor(5,0);
    if (val=="0") {
      lcd.print("CUT");
    } else if (val=="1") {
      lcd.print("CSO");
    } else if (val=="2") {
      lcd.print("SNU");
    } else if (val=="3") {
      lcd.print("OSO");
    }
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/status").c_str())==0) {
    lcd.setCursor(9,0);
    if (val=="bat") {
      lcd.print("SBU");
    } else if (val=="sol") {
      lcd.print("SOL");
    } else if (val=="uti") {
      lcd.print("UTI");
    }
  }
}


float zaokrouhli(float cislo, float desetiny) {
  return round(cislo * pow(10.f, desetiny)) / pow(10.0, desetiny);
}

bool isDebugEnabled() {
#ifdef verbose
  return true;
#endif // verbose
  return false;
}

void printMessageToLCD(char* t, String v) {
  lcd.clear();
  lcd.print(t);
  lcd.print(": ");
  lcd.print(v);
  //delay(2000);
  lcd.clear();
}


WiFiClient espClient;
PubSubClient client(espClient);

WiFiManager wifiManager;

/////////////////////////////////////////////   S  E  T  U  P   ////////////////////////////////////
void setup(void) {
  SERIAL_BEGIN;
  DEBUG_PRINT(F(SW_NAME));
  DEBUG_PRINT(F(" "));
  DEBUG_PRINTLN(F(VERSION));

  pinMode(BUILTIN_LED, OUTPUT);
  ticker.attach(1, tick);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(CONFIG_PORTAL_TIMEOUT);
  wifiManager.setConnectTimeout(CONNECT_TIMEOUT);

  lcd.init();               // initialize the lcd 
  lcd.backlight();
  lcd.home();
  lcd.print(SW_NAME);
  PRINT_SPACE
  lcd.print(VERSION);
  lcd.createChar(0, customChar);


  if (drd.detectDoubleReset()) {
    DEBUG_PRINTLN("Double reset detected, starting config portal...");
    ticker.attach(0.2, tick);
    if (!wifiManager.startConfigPortal(HOSTNAMEOTA)) {
      DEBUG_PRINTLN("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
  }

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

  WiFi.printDiag(Serial);
  
  if (!wifiManager.autoConnect(AUTOCONNECTNAME, AUTOCONNECTPWD)) { 
    DEBUG_PRINTLN("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 

  sendNetInfoMQTT();

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
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
#endif



  lcd.clear();
  
  //setup timers
  timer.every(SENDSTAT_DELAY, sendStatisticMQTT);
#ifdef time  
  timer.every(500,            displayTime);
#endif
  void * a;
  sendStatisticMQTT(a);
  

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);

  drd.stop();

  DEBUG_PRINTLN(F("Setup end."));
}

/////////////////////////////////////////////   L  O  O  P   ///////////////////////////////////////
void loop(void) { 

  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif

  reconnect();
  client.loop();

  display();

 
#ifdef PIR
  if (digitalRead(PIRPIN)==1) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }  
#endif

#ifdef time
  displayClear();
#endif
  
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

bool sendStatisticMQTT(void *) {
  digitalWrite(BUILTIN_LED, LOW);
  //printSystemTime();
  DEBUG_PRINTLN(F("Statistic"));

  SenderClass sender;
  sender.add("VersionWattMeter",              VERSION);
  sender.add("Napeti",  ESP.getVcc());
  sender.add("HeartBeat",                     heartBeat++);
  if (heartBeat % 10 == 0) sender.add("RSSI", WiFi.RSSI());
  
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}


void sendNetInfoMQTT() {
  digitalWrite(BUILTIN_LED, LOW);
  //printSystemTime();
  DEBUG_PRINTLN(F("Net info"));

  SenderClass sender;
  sender.add("IP",              WiFi.localIP().toString().c_str());
  sender.add("MAC",             WiFi.macAddress());
  
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base);
  digitalWrite(BUILTIN_LED, HIGH);
  return;
}



//---------------------------------------------D I S P L A Y ------------------------------------------------
/*
  01234567890123456789
  --------------------
0|56/66 CER      40/45
1|                    
2|                    
3|19:20 15°C     1440m
  --------------------
  01234567890123456789  
  
  
*/
void display() {
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

void printSystemTime(){
  char buffer[20];
  sprintf(buffer, "%02d.%02d.%4d %02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());
  DEBUG_PRINT(buffer);
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


bool displayTime(void *) {
  lcd.setCursor(TIMEX, TIMEY); //col,row
  char buffer[6];
  if (showDoubleDot) {
    sprintf(buffer, "%02d:%02d", hour(), minute());
  } else {
    Wire.write(byte(0));
    sprintf(buffer, "%02d %02d", hour(), minute());
  }
  lcd.print(buffer);
  showDoubleDot = !showDoubleDot;
  return true;
}
#endif

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    if (lastConnectAttempt == 0 || lastConnectAttempt + connectDelay < millis()) {
      DEBUG_PRINT("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(mqtt_base, mqtt_username, mqtt_key)) {
        DEBUG_PRINTLN("connected");
        // Once connected, publish an announcement...
        client.subscribe((String(mqtt_base) + "/#").c_str());
        client.subscribe((String(mqtt_pip2424) + "/#").c_str());
      } else {
        lastConnectAttempt = millis();
        DEBUG_PRINT("failed, rc=");
        DEBUG_PRINTLN(client.state());
      }
    }
  }
}

