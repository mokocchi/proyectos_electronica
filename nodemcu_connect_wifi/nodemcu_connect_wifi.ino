#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include "config.h"

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID_name, SSID_pass);

  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {

}
