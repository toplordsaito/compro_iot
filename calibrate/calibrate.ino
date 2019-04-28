#include <TimeLib.h>
#include "DHT.h"
#include "math.h"

#define DHTPIN D0 // what digital pin we're connected to

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  int sd = calibrate();
  float tempSD;
  float humiSD;
  float disSD
  float temp,humi,dis;

  Serial.begin(9600);
  Serial.println("DHTxx test!");

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
  // put your main code here, to run repeatedly:

}

void calibrate() {
  float tem, dist, humi;

  //Star Value
  tem = dht.readTemperature();
  dist = dht.readHumidity();
  humi = analogRead();

  //Calibrate
  for (int i=0; i<10; i++) {
    tem = analogRead();
    dist = analogRead();
    humi = analogRead();
  }

  temSD = calculateSD(tem);
  distSD = calculateSD(dist);
  humiSD = calculateSD(humi);

}

float calculateSD(float var) {
  float sum = 0.0, mean, standardDeviation = 0.0;
  
  for(int i=0; i<10; ++i) {
        sum += data[i];
  }

  mean = sum/10;

  for(i=0; i<10; ++i)
   standardDeviation += pow(data[i] - mean, 2);
  
  return sqrt(standardDeviation/10);
}

float calculateCToF(float celsius){
  float fahrenheit;
  fahrenheit = 1.80*celsius + 32;
  return fahrenheit;
}
void extension() {
  //for tempulature
  if (tem >= 30) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else if (tem >= 25) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else if (tem >= 20) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  }

  //for humidity
  if (humi >= 60) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%");
    printf("** ความชื้นในอากาศมีค่ามากเกินไปซึ่งป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา เป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา ซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดได้ **");
  } else if (humi >= 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%");
  } else (humi < 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%");
    printf("** ความชื้นในอากาศมีค่าต่ำกว่าระดับที่เหมาะสม ควรใช้ครีมบำรุงผิวเพื่อให้ผิวชุ่มชื้นอยู่ตลอดเวลา **");
  }

  //for check celsius to fahrenheit
  printf("%f", calculateCToF(temp))
}
