#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
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


void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("https://b1677718.ngrok.io/omg");      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST("Message from ESP8266");   //Send the request
   String payload = http.getString();                  //Get the response payload
   Serial.print("httpCode: ");
   Serial.println(httpCode);   //Print HTTP return code
   Serial.print("Payload: ");
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(30000);  //Send a request every 30 seconds
 
}
