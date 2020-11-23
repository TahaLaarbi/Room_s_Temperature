//TAHA LAARBI

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
const int sensorPin = A0;
int reading;
float voltage;
float temperatureC;
float vref = 3.3;
float resolution = vref/1023;
const char* ssid = "Your WiFi";
const char* password = "Your Password";

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {
  USE_SERIAL.begin(9600);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d... \n",t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid , password);
  
}

void loop() {
  float temperature = analogRead(A0);
  temperature = (temperature*resolution);
  temperature = temperature*100;
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    
        HTTPClient http;
    
        USE_SERIAL.print("[HTTP] begin...\n");
        
        http.begin("http://api.thingspeak.com/update?api_key=YOUR API KEY&field1=" +  String(temperature));
    
        USE_SERIAL.print("[HTTP] GET... \n");    
    
        int httpcode = http.GET();
    
        if (httpcode > 0) {
          
          USE_SERIAL.printf("(HTTP) GET... code: %d\n", httpcode);
    
          if (httpcode == HTTP_CODE_OK) {
            String payload = http.getString();
            USE_SERIAL.println(payload);
            USE_SERIAL.println(temperature);
          }
        } else {
          USE_SERIAL.printf("[HTTP] GET... failed, error; %s\n", http.errorToString(httpcode).c_str());
        }
    }
      //To repeat every 5 minutes.
      delay(1000*300);
}
