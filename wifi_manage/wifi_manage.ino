#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
void setup() 
{
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
}
void loop() 
{
}
