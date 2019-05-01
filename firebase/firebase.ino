#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
 
#define WIFI_SSID "Beaslzlo"
#define WIFI_PASSWORD "0911919890"
#define Red D0
#define Green D1
#define Blue D2
int i = 0;
#define FIREBASE_HOST "iot-itcamp-4faf6.firebaseio.com"
#define FIREBASE_AUTH "SIfGol16ty51iMyjzndxwuaZvyM7ymdfdFcvHdnW"
int red, green, blue;
void setup() {
  Serial.begin(9600);
  // connect to wifi.
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
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
 
}
 
void loop() {
  Firebase.set("test/code",2);
  Firebase.set("test/val",i);
  delay(1000);
  i+=1;
// Firebase.set("test1/gg", 9999);
// red = Firebase.getBool("Red");
// green = Firebase.getBool("Green");
// blue = Firebase.getBool("Blue");
// digitalWrite(Red, !red);
// digitalWrite(Green, !green);
// digitalWrite(Blue, !blue);
// Serial.print("Red: ");
// Serial.println(red);
// Serial.print("Greens: ");
// Serial.println(green);
// Serial.print("Blue: ");
// Serial.println(blue);
  delay(100);
}
