//#include <TimeLib.h>
#include "DHT.h"
#include "math.h"
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#define DHTPIN D3 // what digital pin we're connected to
#define TRIC D0 //input distance sensor
#define ECHO D1 //output distance sensor
#define DHTTYPE DHT11
#define VARIANCE 10 //ความแปรปวน
#define LED D5
#define WIFI_SSID "Beaslzlo"
#define WIFI_PASSWORD "0911919890"
#define FIREBASE_HOST "iot-itcamp-4faf6.firebaseio.com"
#define FIREBASE_AUTH "SIfGol16ty51iMyjzndxwuaZvyM7ymdfdFcvHdnW"

DHT dht(DHTPIN, DHTTYPE);
String error = "";
float tempSD;
float humiSD;
float distSD;
float temp[10], humi[10], dist[10];
float tempNow, humiNow, distNow;      //ค่าที่จะอัพขึ้น
bool LED_status;
void setup() {
  // connect to wifi.
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  pinMode(LED, OUTPUT);
  pinMode(TRIC, OUTPUT);  
  pinMode(ECHO, INPUT);  

  dht.begin();
  calibrate();
}

void loop() {
  for (int i=0; i<10; i++) {
    temp[i] = dht.readTemperature();
    dist[i] = find_dist();
    humi[i] = dht.readHumidity();
    Serial.print("temp : ");
    Serial.println(temp[i]);
    Serial.print("Humi : ");
    Serial.println(humi[i]);
    Serial.print("Dis : ");
    Serial.println(dist[i]);
    delay(2000);
  }

  tempNow = temp[9]; 
  distNow = dist[9]; 
  humiNow = humi[9];

  float temporary = calculateSD(temp);
  Serial.println(temporary);
  tempSD = abs(temporary - tempSD) < VARIANCE ? temporary: tempSD;
  
  temporary = calculateSD(dist);
  Serial.println(temporary);
  distSD = abs(temporary - distSD) < VARIANCE ? temporary: distSD;
  
  temporary = calculateSD(humi);
  Serial.println(temporary);
  humiSD = abs(temporary - humiSD) < VARIANCE ? temporary: humiSD;
  
  Serial.print("tempSD: ");
  Serial.println(tempSD);
  Serial.print("distSD: ");
  Serial.println(distSD);
  Serial.print("humiSD: ");
  Serial.println(humiSD);
  Firebase.setString("error", error);
  Firebase.set("temp", tempNow);
  Firebase.set("dis", distNow);
  Firebase.set("humi", humiNow);
  Firebase.set("tempSD", tempSD);
  Firebase.set("disSD", distSD);
  Firebase.set("humiSD", humiSD);
  LED_status = Firebase.getBool("LED");
  Serial.print("LED status : ");
  Serial.println(LED_status);
  digitalWrite(LED, LED_status);
  delay(10000);
//  extension();
}

void calibrate() {
  //Star Value
  tempNow = dht.readTemperature();
  distNow = find_dist();
  humiNow = dht.readHumidity();

  if (isnan(tempNow) || isnan(humiNow)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(500);
    calibrate();
    return;
  }

  //Calibrate
  for (int i=0; i<10; i++) {
    
    temp[i] = dht.readTemperature();
    dist[i] = find_dist();
    humi[i] = dht.readHumidity();
    Serial.print(".");
    delay(500);
  }
  Serial.println("init OK");
  tempSD = calculateSD(temp);
  distSD = calculateSD(dist);
  humiSD = calculateSD(humi);
  tempNow = temp[9]; 
  distNow = dist[9]; 
  humiNow = humi[9];
}

float calculateSD(float data[10]) {
  float sum = 0.0, mean, standardDeviation = 0.0;
  
  for(int i=0; i<10; ++i) {
        sum += data[i];
  }

  mean = sum/10;

  for(int i=0; i<10; ++i)
   standardDeviation += pow(data[i] - mean, 2);
  
  return sqrt(standardDeviation/10);
}

float calculateCToF(float celsius){
  float fahrenheit;
  fahrenheit = 1.80*celsius + 32;
  return fahrenheit;
}

float find_dist() {
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


//Alert part

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
  } else if(humiNow < 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%", humiNow);
    printf("** ความชื้นในอากาศมีค่าต่ำกว่าระดับที่เหมาะสม ควรใช้ครีมบำรุงผิวเพื่อให้ผิวชุ่มชื้นอยู่ตลอดเวลา **");
  }
}
