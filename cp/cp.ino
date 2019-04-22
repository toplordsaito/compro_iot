void Line_Notify(String message) ;
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Config Firebase
#define FIREBASE_HOST "iot-itcamp-4faf6.firebaseio.com"
#define FIREBASE_AUTH "SIfGol16ty51iMyjzndxwuaZvyM7ymdfdFcvHdnW"

// Config connect WiFi
#define WIFI_SSID "Chaiyadith"
#define WIFI_PASSWORD "0805388289"
#define LINE_TOKEN "8c9nBvtni4G1bnR2sBLvdPpygIkMpw6YmaVfIcP6Nrj"
String message = "เปิด";
int i = 0;
bool LED1, LED2;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  // connect to wifi.
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
  Firebase.setInt("number", i);
  if (Firebase.failed()) {
      Serial.print("set /number failed:");
      Serial.println(Firebase.error());  
  }
  Serial.print("set /number to ");
  Serial.println(Firebase.getInt("number"));
  LED1 = Firebase.getBool("LED1");
  LED2 = Firebase.getBool("LED2");
  Serial.print("LED1 : ");
  Serial.println(LED1);
  Serial.print("LED 2: ");
  Serial.println(LED2);
  if(LED1){
    Line_Notify(message);
    }
  digitalWrite(D1, LED1);
  digitalWrite(D2, LED2);
  
  i++;
  delay(500);
}


void Line_Notify(String message) {
  WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
