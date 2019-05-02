#include <TimeLib.h>
#include "DHT.h"
#include "math.h"
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

#define DHTPIN D3 // what digital pin we're connected to
#define TRIC D0 //input distance sensor
#define ECHO D1 //output distance sensor
#define DHTTYPE DHT11
#define VARIANCE 2 //ความแปรปวน
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
bool LED_status, LED_BEFORE;
int timeNow;
String codeNow = "";

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = 7;     // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
int pass = 0;
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);


time_t prevDisplay = 0; // when the digital clock was displayed

void setup() {
  // connect to wifi.
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);    
  if(!wifiManager.autoConnect("AutoConnectAP")) 
  {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  WiFi.printDiag(Serial);
  Serial.println();
  Serial.println("connected...OK"); 
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  pinMode(LED, OUTPUT);
  pinMode(TRIC, OUTPUT);  
  pinMode(ECHO, INPUT);  
  Firebase.setBool("val/LED", true);

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  dht.begin();
  calibrate();
  
  LED_status = Firebase.getBool("val/LED");
  LED_BEFORE = LED_status;
  timeNow = hour();

  Serial.println("Starting create Data JSON");
  Serial.println(timeNow);
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["time"] =String(timeNow) + ".00 " + String(day()) + "/" + String(month()) + "/" + String(year());
  root["Temp"] = tempNow;
  root["Humi"] = humiNow;
  Firebase.push("DataByTime", root);
}

void loop() {
  if (hour() == 6 && minute() == 0) {
      alert("six");
      delay(60000);
    }
    
  for (int i=0; i<10; i++) {
    ledManage();
    tempNow = dht.readTemperature();
    distNow = find_dist();
    humiNow = dht.readHumidity();
    temp[i] = tempNow;
    dist[i] = distNow;
    humi[i] = humiNow;
    Firebase.set("val/temp", tempNow);
    Firebase.set("val/dis", distNow);
    Firebase.set("val/humi", humiNow);
    Serial.print("temp : ");
    Serial.println(tempNow);
    Serial.print("Humi : ");
    Serial.println(humiNow);
    Serial.print("Dis : ");
    Serial.println(distNow);
    if(distNow < 20){
        pass += 1;
        Firebase.set("pass", pass);
    }
    delay(1000);
  }

  //Temporary SD
  Serial.println("''''''  Standard Deviation  '''''' ");
  float temporary = calculateSD(temp);
  Serial.println(temporary);
  if ((abs(temporary - tempSD)) < VARIANCE) {
    tempSD = temporary;
  } else {
    alert("temp");
  }
  
  
  temporary = calculateSD(humi);
  Serial.println(temporary);
  if ((abs(temporary - humiSD)) < VARIANCE) {
    humiSD = temporary;
  } else {
    alert("humi");
  }
  
  Serial.print("tempSD: ");
  Serial.println(tempSD);
  Serial.print("distSD: ");
  Serial.println(distSD);
  Serial.print("humiSD: ");
  Serial.println(humiSD);
  Firebase.setString("error", error);
  
  Firebase.set("SD/tempSD", tempSD);
  Firebase.set("SD/disSD", distSD);
  Firebase.set("SD/humiSD", humiSD);

  if (timeNow != hour()) {
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["time"] =String(timeNow) + ".00 " + String(day()) + "/" + String(month()) + "/" + String(year());
  root["Temp"] = tempNow;
  root["Humi"] = humiNow;
  Firebase.push("DataByTime", root);
  timeNow = hour();
  }
 extension();
}

void ledManage() {
  LED_status = Firebase.getBool("val/LED");

  if (LED_BEFORE != LED_status) {
  Serial.print("LED status : ");
  Serial.println(LED_status);
  digitalWrite(LED, LED_status);
  LED_BEFORE = LED_status;
  }
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
    ledManage();
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

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}



//Alert part

int alert(String code) {

  Serial.println("----Before----");
  Serial.print("Code: ");
  Serial.println(code);
  Serial.print("CodeNow: ");
  Serial.println(codeNow);

  if (code != codeNow) {
   StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  if (code == "temp") {
    root["code"] = 0;
  } else if (code == "hot") {
    root["code"] = 0.1;
  } else if (code == "cold") {
    root["code"] = 0.2;
  } else if (code == "hi_humi") {
    root["code"] = 1.1;
  } else if (code == "me_humi") {
    root["code"] = 1.2;
  } else if (code == "lo_humi") {
    root["code"] = 1.3;
  } else if (code == "dist") {
    root["code"] = 2;
  } else if (code == "six") {
    root["code"] = 3;
  }

  root["temp"] = tempNow;
  root["humi"] = humiNow;
  root["time"] = String(day()) + "/" + String(month()) + "/" + String(year());

  Firebase.set("Code", root);
  codeNow = code;
  }
  
  Serial.println();
  Serial.println("----After----");
  Serial.print("Code: ");
  Serial.println(code);
  Serial.print("CodeNow: ");
  Serial.println(codeNow);
  
  
}

void extension() {
  //for tempulature
  if (tempNow >= 35) {
    alert("hot");
  } else if (tempNow < 10) {
    alert("cold");
  }

  //for humidity
  if (humiNow >= 60) {
    alert("hi_humi");
  } else if (humiNow >= 30) {
    alert("me_humi");
  } else if(humiNow < 30) {
    alert("lo_humi");
  }
}
