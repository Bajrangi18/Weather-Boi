#include <WiFi.h>
#include "time.h"
#include <FirebaseESP32.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"

#define DHTPIN 15  
#define FIREBASE_HOST "https://permafrost-db-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "BV7OWDWrp3Tja7TXqqlYm2s0EXvbD07RNny7iBGW"
#define DHTTYPE DHT22 
FirebaseData firebaseData;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

char h[100];
void setup()
{
Serial.begin(9600);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("ACT102541158955","68594361");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  configTime(19800,0,"pool.ntp.org");
   if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X8,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

 
 dht.begin();
}


void loop()
{   sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);
    String temp_go = String(temp_event.temperature);
    String pres_go = String(pressure_event.temperature);
    String humid_go = String(dht.readHumidity());
    printLocalTime();
    Firebase.setString(firebaseData,h,"Weather");
    Firebase.pushString(firebaseData,h,temp_go);
    Firebase.pushString(firebaseData,h,pres_go);
    Firebase.pushString(firebaseData,h,humid_go);
    delay(1000);
}
void printLocalTime()
{
  String s = "";
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime (h, 100, "%A %B %d %H:%M:%S %Y", &timeinfo);
  
}
