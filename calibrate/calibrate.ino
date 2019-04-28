#include <TimeLib.h>
#include "DHT.h"
#include "math.h"

#define DHTPIN D0 // what digital pin we're connected to
#define TRIC D1 //input distance sensor
#define ECHO D2 //output distance sensor
#define DHTTYPE DHT11
#define VARIANCE 10 //ความแปรปวน 

DHT dht(DHTPIN, DHTTYPE);

float tempSD;
float humiSD;
float distSD;
float temp[10], humi[10], dist[10];
float tempNow, humiNow, distNow;      //ค่าที่จะอัพขึ้น

void setup() {
  Serial.begin(9600);
  pinMode(TRIC, OUTPUT);  
  pinMode(ECHO, INPUT);  

  calibrate();

  dht.begin();
}

void loop() {
  for (int i=0; i<10; i++) {
    temp[i] = dht.readTemperature();
    dist[i] = analogRead();
    humi[i] = dht.readHumidity();
    delay(10000);
  }
  
  tempSD = abs((calculateSD(tem)/100)*tempSD - 100) < VARIANCE ? (calculateSD(tem)): tempSD;
  distSD = abs((calculateSD(dist)/100)*tempSD - 100) < VARIANCE ? (calculateSD(dist)): tempSD;
  humiSD = abs((calculateSD(humi)/100)*tempSD - 100) < VARIANCE ? (calculateSD(humi)): tempSD;

  extension();
}

void calibrate() {
  //Star Value
  tempNow = dht.readTemperature();
  distNow = analogRead();
  humiNow = dht.readHumidity();

  if (isnan(tempNow) || isnan(humiNow)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(500);
    calibrate();
  }

  //Calibrate
  for (int i=0; i<10; i++) {
    temp[i] = dht.readTemperature();
    dist[i] = analogRead();
    humi[i] = dht.readHumidity();
    delay(10000);
  }
  
  tempSD = calculateSD(temp);
  distSD = calculateSD(dist);
  humiSD = calculateSD(humi);

  tempNow = temp[9] 
  distNow = dist[9] 
  humiNow = humi[9]
}

float calculateSD(float data[10]) {
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
  if (tempNow >= 30) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", tempNow, calculateCToF(tempNow));
  } else if (tempNow >= 25) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", tempNow, calculateCToF(tempNow));
  } else if (tempNow >= 20) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", tempNow, calculateCToF(tempNow));
  } else {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", tempNow, calculateCToF(tempNow));
  }

  //for humidity
  if (humiNow >= 60) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humiNow);
    printf("** ความชื้นในอากาศมีค่ามากเกินไปซึ่งป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา เป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา ซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดได้ **");
  } else if (humiNow >= 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humiNow);
  } else (humiNow1 < 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humiNow);
    printf("** ความชื้นในอากาศมีค่าต่ำกว่าระดับที่เหมาะสม ควรใช้ครีมบำรุงผิวเพื่อให้ผิวชุ่มชื้นอยู่ตลอดเวลา **");
  }
}

void find_dist() {
  float duration, cm;

  digitalWrite(TRIC, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIC, LOW);
  
  duration = pulseIn(D1, HIGH);
  cm = (duration/2)/29.1;

  return cm;

}
