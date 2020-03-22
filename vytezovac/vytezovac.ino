#include "Configuration.h"

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#include "Sender.h"

//for LED status
Ticker ticker;

float solarOUTVoltage                        = 0.f;
int manualRelay                              = 2;
uint32_t heartBeat                           = 0;

byte relayStatus                             = RELAY_OFF;


bool isDebugEnabled()
{
#ifdef verbose
  return true;
#endif // verbose
  return false;
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
  
  if (strcmp(topic, (String(mqtt_base_vytezovac) + "/" + String(mqtt_topic_relay)).c_str())==0) {
    DEBUG_PRINT("set manual control relay to ");
    manualRelay = val.toInt();
    if (val.toInt()==1) {
      DEBUG_PRINTLN(F("ON"));
    } else if (val.toInt()==0) {
      DEBUG_PRINTLN(F("OFF"));
    } else {
      DEBUG_PRINTLN(F("AUTO"));
    }
  } else if (strcmp(topic, (String(mqtt_base_solar) + "/" + String(mqtt_topic_voltage)).c_str())==0) {
    solarOUTVoltage = val.toFloat();
  } else if (strcmp(topic, (String(mqtt_base_vytezovac) + "/" + String(mqtt_topic_restart)).c_str())==0) {
    DEBUG_PRINT("RESTART");
    ESP.restart();
  }
}

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  SERIAL_BEGIN;
  DEBUG_PRINT(F(SW_NAME));
  DEBUG_PRINT(F(" "));
  DEBUG_PRINTLN(F(VERSION));

  pinMode(LOAD, OUTPUT);
  digitalWrite(LOAD, 0);

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

  
  //setup timers
  //timer.every(SEND_DELAY, sendDataHA);
  timer.every(SENDSTAT_DELAY, sendStatisticHA);
  void * a;
  sendStatisticHA(a);

  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(STATUS_LED, HIGH);
}

void loop() {
  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif


  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  control();
}


void control() {
  DEBUG_PRINT(manualRelay);
  DEBUG_PRINTLN(relayStatus);
  if (manualRelay==2) {
    // if (relayStatus == RELAY_OFF && solarOUTVoltage > VOLTAGEON) {
      // DEBUG_PRINTLN("Relay ON");
      // digitalWrite(LOAD, 1);
      // relayStatus = RELAY_ON;
      // sendRelayHA(1);
    // }
    // if (relayStatus == RELAY_ON && solarOUTVoltage < VOLTAGEOFF) {
      // DEBUG_PRINTLN("Relay OFF");
      // digitalWrite(LOAD, 0);
      // relayStatus = RELAY_OFF;
      // sendRelayHA(0);
    // }
  } else if (relayStatus == RELAY_OFF && manualRelay==1) {
      DEBUG_PRINTLN("Relay ON");
      digitalWrite(LOAD, 1);
      relayStatus = RELAY_ON;
      sendRelayHA(1);
  } else if (relayStatus == RELAY_ON && manualRelay==0) {
      DEBUG_PRINTLN("Relay OFF");
      digitalWrite(LOAD, 0);
      relayStatus = RELAY_OFF;
      sendRelayHA(0);
  }
}

void sendRelayHA(byte akce) {
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  SenderClass sender;
  sender.add("relayChangeVytezovac", akce);
 
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base_vytezovac);
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
}


bool sendStatisticHA(void *) {
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  //printSystemTime();
  DEBUG_PRINTLN(F(" - I am sending statistic to HA"));

  SenderClass sender;
  sender.add("VersionSWVytezovac", VERSION);
  sender.add("HeartBeat", heartBeat++);
  sender.add("RSSI", WiFi.RSSI());
  
  DEBUG_PRINTLN(F("Calling MQTT"));
  
  sender.sendMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_key, mqtt_base_vytezovac);
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
  return true;
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_base_vytezovac, mqtt_username, mqtt_key)) {
      DEBUG_PRINTLN("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      // ... and resubscribe
      //client.subscribe(mqtt_base + '/' + 'inTopic');
       client.subscribe((String(mqtt_base_vytezovac) + "/" + String(mqtt_topic_relay)).c_str());
       client.subscribe((String(mqtt_base_vytezovac) + "/" + String(mqtt_topic_restart)).c_str());
       client.subscribe((String(mqtt_base_solar) + "/" + String(mqtt_topic_voltage)).c_str());
    } else {
      DEBUG_PRINT("failed, rc=");
      DEBUG_PRINT(client.state());
      DEBUG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
