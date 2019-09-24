#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define button_pin D3

ESP8266WiFiMulti WiFiMulti;
int prev_status;
int button_cnt = 0;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "PASSWORD");

  pinMode(button_pin, INPUT);
  prev_status = digitalRead(button_pin);
}

void loop() {

  if ((WiFiMulti.run() == WL_CONNECTED) && (prev_status != digitalRead(button_pin))) {

    int buttonstatus = digitalRead(button_pin);
    /* Count when pressed */  
    if(buttonstatus == 0){
      button_cnt++; 
    }

    HTTPClient http;
    http.begin("http://<SERVER-IP-ADDRESS>:3030/buttonstatus");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String params = "buttonstatus=" + String(buttonstatus);
           params += "&";
           params += "buttoncnt=" + String(button_cnt);
    
    int httpCode = http.POST(params);

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    prev_status = buttonstatus;
  }

  delay(500);

}
