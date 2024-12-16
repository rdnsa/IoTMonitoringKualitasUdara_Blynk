#define BLYNK_TEMPLATE_ID "TMPL6Nxe8LTFA"
#define BLYNK_TEMPLATE_NAME "Monitoring Kualitas Udara"
#define BLYNK_AUTH_TOKEN "jSSXkKqKoORxIG6m10b8j7-r_MzeCIJ2"

#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi Note 9";
char pass[] = "issaganteng";

#include <MQ135.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR   0x3C
#define PIN_MQ135 A0
#define DHTPIN D4 
#define DHTTYPE DHT11

MQ135 mq135_sensor(PIN_MQ135);
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);
DHT dht(DHTPIN, DHTTYPE);
float temperature = 21.0; 
float humidity = 25.0; 
float rzero;
float correctedRZero;
float resistance;
float ppm;
float correctedPPM;
float humi, temp; 

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  delay(10);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("NodeMCU V3"); 
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println("Monitoring");
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,35);
  display.println("Kualitas Udara");
  display.display();  
  Blynk.begin(auth, ssid, pass);
  dht.begin();   
  delay(2000);  
}

void readDHT11()
{
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humi) || isnan(temp)) {
    Serial.println("DHT11 tidak terbaca... !");
    return;
  }
  else{
  Serial.print("Suhu=");  
  Serial.print(temp);
  Serial.println(" Celsius");
  Serial.print("Humi=");
  Serial.print(humi);   
  Serial.println(" %RH");
  Serial.println();
  }  
}

void readMQ135()
{
  rzero = mq135_sensor.getRZero();
  correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  resistance = mq135_sensor.getResistance();
  ppm = mq135_sensor.getPPM();
  correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
  
  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");
}

void loop() {
  readMQ135();
  readDHT11();

  Blynk.virtualWrite(V0, correctedPPM); 
  Blynk.virtualWrite(V1, temp);      
  Blynk.virtualWrite(V2, humi);  

  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");
  
  display.clearDisplay();
  display.setCursor(0,16); 
  display.setTextSize(2);
  display.setTextColor(WHITE); 
}