#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "time.h"

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

const char* ssid       = "surya";
const char* password   = "12345678";

//the server in.pool.ntp.org is for indian time zone but i don't find it working for that i have to add something in line 15
const char* ntpServer = "in.pool.ntp.org";
const long  gmtOffset_sec = 19800; // the offset time needs to be increased so that the time shows exactlty according to the IST.
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    lcd.setCursor(0, 0);
    lcd.print("Failed try");
    delay(1000);
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
   lcd.setCursor(0, 0);
   lcd.print(&timeinfo, "%A, %B %d");
   lcd.setCursor(0, 1);
   lcd.print(&timeinfo, "%Y %H:%M:%S");
   delay(1000);
}

void setup()
{
  Serial.begin(115200);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      lcd.setCursor(0, 0);
      lcd.print("Connecting...");
      Serial.print(".");
      delay(1000);
      lcd.clear();
  }
  Serial.println(" CONNECTED");
  lcd.setCursor(0, 0);
  lcd.print("Connected...");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  //delay(1000);
  printLocalTime();
  lcd.setCursor(0, 0);
  printLocalTime();
  lcd.clear();
}
