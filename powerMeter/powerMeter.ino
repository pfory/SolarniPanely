/*
--------------------------------------------------------------------------------------------------------------------------
POWERMETER - shows values from FV
Petr Fory pfory@seznam.cz
GIT - https://github.com/pfory/
*/

#include "Configuration.h"

bool                  dispClear                   = false;
bool                  vytezovac                   = false;
uint8_t               teplotaBojler               = 0;
uint32_t              lastPulse1                  = 0;

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

//MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  // char * pEnd;
  // long int valL;
  String val =  String();
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  for (unsigned int i=0;i<length;i++) {
    DEBUG_PRINT((char)payload[i]);
    val += (char)payload[i];
  }
  DEBUG_PRINTLN();
  
  if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_restart)).c_str())==0) {
    DEBUG_PRINT("RESTART");
    ESP.restart();
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_topic_netinfo)).c_str())==0) {
    sendNetInfoMQTT();
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/masterstatus").c_str())==0) {
    lcd.setCursor(0,0);
    if (val=="Line") {
      lcd.print("LIN");
    } else if (val=="Battery") {
      lcd.print("BAT");
    }
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/charge").c_str())==0) {
    lcd.setCursor(4,0);
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
    lcd.setCursor(8,0);
    if (val=="bat") {
      lcd.print("SBU");
    } else if (val=="sol") {
      lcd.print("SOL");
    } else if (val=="uti") {
      lcd.print("UTI");
    }
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/pvEstimateToday").c_str())==0) {
    lcd.setCursor(0,1);
    char temp[5];
    snprintf (temp,5,"%1.2f",val.toFloat());
    lcd.print(temp);
    lcd.print("/");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/pvEstimateTomorow").c_str())==0) {
    lcd.setCursor(5,1);
    char temp[5];
    snprintf (temp,5,"%1.2f",val.toFloat());
    lcd.print(temp);
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/powerFVToday").c_str())==0) {
    lcd.setCursor(0,2);
    char temp[5];
    snprintf (temp,5,"%1.2f",val.toFloat());
    lcd.print(temp);
    lcd.print("/");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/utiToday").c_str())==0) {
    lcd.setCursor(5,2);
    char temp[5];
    snprintf (temp,5,"%2.2f",val.toFloat());
    lcd.print(temp);
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/pvchargew").c_str())==0) {
    lcd.setCursor(0,3);
    char temp[5];
    snprintf (temp,5,"%4d",(int)val.toInt());
    lcd.print(temp);
    lcd.print("/");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/acoutw").c_str())==0) {
    lcd.setCursor(5,3);
    char temp[5];
    snprintf (temp,5,"%4d",(int)val.toInt());
    lcd.print(temp);
    lcd.print("/");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/utiPower").c_str())==0) {
    lcd.setCursor(10,3);
    char temp[5];
    snprintf (temp,5,"%4d",(int)val.toInt());
    lcd.print(temp);
    lcd.print("W");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/battcappa").c_str())==0) {
    lcd.setCursor(16,3);
    char temp[4];
    snprintf (temp,4,"%3d",(int)val.toInt());
    lcd.print(temp);
    lcd.print("%");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/battv").c_str())==0) {
    lcd.setCursor(15,2);
    char temp[5];
    snprintf (temp,5,"%3.1f",val.toFloat());
    lcd.print(temp);
    lcd.print("V");
  } else if (strcmp(topic, (String(mqtt_pip2424) + "/load").c_str())==0) {
    lcd.setCursor(10,2);
    char temp[4];
    snprintf (temp,4,"%3d",(int)val.toInt());
    lcd.print(temp);
    lcd.print("%");
  } else if (strcmp(topic, (String(mqtt_bojler) + "/tBojler2").c_str())==0) {
    teplotaBojler = (int)val.toInt();
  } else if (strcmp(topic, (String(mqtt_solarEnergyMeter) + "/pulseLength1").c_str())==0) {
    lastPulse1 = millis();
    lcd.setCursor(10,1);
    char temp[5];
    snprintf (temp,5,"%4d", (int)(3600.f/((float)val.toInt()/1000.f)));
    lcd.print(temp);
  } else if (strcmp(topic, (String(mqtt_solarEnergyMeter) + "/pulseLength2").c_str())==0) {
    lcd.setCursor(14,1);
    lcd.print("/");
    char temp[5];
    snprintf (temp,5,"%4d", (int)(3600.f/((float)val.toInt()/1000.f)));
    lcd.print(temp);
    lcd.print("W");
  } else if (strcmp(topic, (String(mqtt_vytezovac) + "/POWER").c_str())==0) {
    if (val=="ON") {
      vytezovac = true;
    } else {
      vytezovac = false;
    }
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_config_portal)).c_str())==0) {
    startConfigPortal();
  } else if (strcmp(topic, (String(mqtt_base) + "/" + String(mqtt_config_portal_stop)).c_str())==0) {
    stopConfigPortal();
  }
}

/////////////////////////////////////////////   S  E  T  U  P   ////////////////////////////////////
void setup(void) {
  preSetup();
  
  pinMode(VYTEZOVAC_LED, OUTPUT);
  
  lcd.init();               // initialize the lcd 
  lcd.backlight();
  lcd.home();
  lcd.print(SW_NAME);
  PRINT_SPACE
  lcd.print(VERSION);
  lcd.createChar(0, customChar);

  lcd.clear();
  
  //setup timers
  timer.every(CONNECT_DELAY,  reconnect);
#ifdef cas  
  timer.every(500,            displayTime);
#endif
  void * a=0;
  reconnect(a);
  postSetup();
}

/////////////////////////////////////////////   L  O  O  P   ///////////////////////////////////////
void loop(void) { 

  timer.tick(); // tick the timer
#ifdef ota
  ArduinoOTA.handle();
#endif
  client.loop();
  wifiManager.process();  
 
#ifdef PIR
  if (hour()>=22 || hour() <= 6) {
    if (digitalRead(PIRPIN)==1) {
      lcd.backlight();
    } else {
      lcd.noBacklight();
    } 
  } else {
    lcd.backlight();
  }
#endif

#ifdef cas
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

#ifdef cas

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
  
  if (vytezovac==true) {
    digitalWrite(VYTEZOVAC_LED, LOW);
    if (showDoubleDot) {
      lcd.setCursor(12,0);
      lcd.print("  ");
    } else {
      lcd.setCursor(12,0);
      char temp[3];
      snprintf (temp,4,"%2d", teplotaBojler);
      lcd.print(temp);
    }
  } else {
    digitalWrite(VYTEZOVAC_LED, HIGH);
    lcd.setCursor(12,0);
    char temp[3];
    snprintf (temp,4,"%2d", teplotaBojler);
    lcd.print(temp);
  }
  
  if (millis() - lastPulse1 > 250000) {
    lcd.setCursor(10,1);
    lcd.print("   0");

  }
  
  return true;
}
#endif

bool reconnect(void *) {
  if (!client.connected()) {
    DEBUG_PRINT("Attempting MQTT connection...");
    if (client.connect(mqtt_base, mqtt_username, mqtt_key, (String(mqtt_base) + "/LWT").c_str(), 2, true, "offline", true)) {
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_restart)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_topic_netinfo)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_config_portal_stop)).c_str());
      client.subscribe((String(mqtt_base) + "/" + String(mqtt_config_portal)).c_str());
      client.subscribe((String(mqtt_pip2424) + "/#").c_str());
      client.subscribe((String(mqtt_vytezovac) + "/#").c_str());
      client.subscribe((String(mqtt_bojler) + "/tBojler2").c_str());
      client.subscribe((String(mqtt_solarEnergyMeter) + "/pulseLength1").c_str());
      client.subscribe((String(mqtt_solarEnergyMeter) + "/pulseLength2").c_str());
      client.publish((String(mqtt_base) + "/LWT").c_str(), "online", true);
      DEBUG_PRINTLN("connected");
    } else {
      DEBUG_PRINT("disconected.");
      DEBUG_PRINT(" Wifi status:");
      DEBUG_PRINT(WiFi.status());
      DEBUG_PRINT(" Client status:");
      DEBUG_PRINTLN(client.state());
    }
  }
  return true;
}