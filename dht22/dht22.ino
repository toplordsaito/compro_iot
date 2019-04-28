#include "DHT.h"
#include "math.h"
#define DHTPIN D2 // what digital pin we're connected to
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(9600);
Serial.println("DHTxx test!");
  float tempSD;
  float humiSD;
  float disSD
  float temp,humi,dis;

  float percent = abs((temp/100)*tempSD);

dht.begin();
}

void loop() {
  
// หน่วงเวลา 2 วินาทีให้เซนเซอร์ทำงาน
delay(2000);
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);

// เช็คถ้าอ่านค่าไม่สำเร็จให้เริ่มอ่านใหม่
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" *C ");
Serial.print(f);
Serial.print(" *F\t\n");
}
