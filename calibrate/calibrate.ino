#include <TimeLib.h>
#include "DHT.h"
#include "math.h"

#define DHTPIN D0 // what digital pin we're connected to
#define DHTTYPE DHT22
#define VARIANCE 10 //ความแปรปวน 

DHT dht(DHTPIN, DHTTYPE);

float tempSD;
float humiSD;
float distSD;
float temp[10], humi[10], dist[10];

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

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
  temp[0] = dht.readTemperature();
  dist[0] = analogRead();
  humi[0] = dht.readHumidity();

  if (isnan(tem) || isnan(humi)) {
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
  if (temp[9] >= 30) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", temp[9], calculateCToF(temp[9]));
  } else if (temp[9] >= 25) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", temp[9], calculateCToF(temp[9]));
  } else if (temp[9] >= 20) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", temp[9], calculateCToF(temp[9]));
  } else {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ", temp[9], calculateCToF(temp[9]));
  }

  //for humidity
  if (humi[9] >= 60) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humi[9]);
    printf("** ความชื้นในอากาศมีค่ามากเกินไปซึ่งป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา เป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา ซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดได้ **");
  } else if (humi[9] >= 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humi[9]);
  } else (humi[9]1 < 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humi[9]);
    printf("** ความชื้นในอากาศมีค่าต่ำกว่าระดับที่เหมาะสม ควรใช้ครีมบำรุงผิวเพื่อให้ผิวชุ่มชื้นอยู่ตลอดเวลา **");
  }
}
